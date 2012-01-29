using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;

namespace IguanaRC5
{
    /// <summary>
    /// Driver for the IguanaWorks command-line client
    /// </summary>
    internal class IguanaClient
    {
        /// <summary>
        /// Set the IguanaClient carrier frequency
        /// </summary>
        /// <remarks>
        /// Code transmission will fail if the pulse sizes are not an exact
        /// multiple of the carrier wavelength.
        /// </remarks>
        public void SetCarrierFrequency(int carrierFrequency)
        {
            InvokeIgClient(string.Format("--set-carrier={0}", carrierFrequency));
        }

        /// <summary>
        /// Transmit the given sequence of pulses and pauses
        /// </summary>
        public void TransmitSequence(IEnumerable<Segment> sequence)
        {
            var norm  = Normalize(sequence);

            Thread.Sleep(InitialWaitMs(norm));
            SendToClient(FromFirstPulse(norm));
        }

        /// <summary>
        /// Returns the given sequence with consecutive Segments with the same
        /// On value combined.
        /// </summary>
        /// <remarks>
        /// Necessary because the IguanaClient expects alternating pulse/pause commands
        /// </remarks>
        private IEnumerable<Segment> Normalize(IEnumerable<Segment> sequence)
        {
            return sequence
                .SequentialGroupBy(seg => seg.On)
                .Select(grp => new Segment(grp.Item1, grp.Item2.Sum(seg => seg.Microseconds)));
        }

        /// <summary>
        /// Return the initial "dead space" from the sequence in ms
        /// </summary>
        private int InitialWaitMs(IEnumerable<Segment> sequence)
        {
            var waits = sequence.TakeWhile(seg => !seg.On);
            if (waits.Count() == 0) return 0;

            var usec  = waits.Sum(seg => seg.Microseconds);
            return Math.Max(1, (int)Math.Round((double)sequence.ElementAt(0).Microseconds / 1000));
        }

        /// <summary>
        /// Return the sequence starting at the first pulse
        /// </summary>
        private IEnumerable<Segment> FromFirstPulse(IEnumerable<Segment> sequence)
        {
            return sequence.SkipWhile(seg => !seg.On);
        }

        /// <summary>
        /// Have the given sequence executed by the IgClient
        /// </summary>
        private void SendToClient(IEnumerable<Segment> sequence)
        {
            if (sequence.Count() == 0) return;

            using (var file = new TempFile())
            {
                WritePulseFile(file.Path, sequence);
                InvokeIgClient(string.Format("--send=\"{0}\"", file.Path));
            }
        }

        /// <summary>
        /// Write the sequence to the given file
        /// </summary>
        private void WritePulseFile(string path, IEnumerable<Segment> sequence)
        {
            using (var output = new StreamWriter(path))
            {
                sequence.Each(seg =>
                    output.WriteLine("{0} {1}", seg.On ? "pulse" : "space", seg.Microseconds));
            }
        }

        /// <summary>
        /// Run the IguanaClient with the given arguments, raising an exception in case of error
        /// </summary>
        private void InvokeIgClient(string arguments)
        {
            var psi = new ProcessStartInfo(IgClientPath(), arguments)
            {
                UseShellExecute = false,
                CreateNoWindow = true,
                RedirectStandardError = true
            };

            var p = Process.Start(psi);
            var err = p.StandardError.ReadToEnd();
            p.WaitForExit();

            if (p.ExitCode != 0) throw new InvalidOperationException("igclient failed: " + err);
        }

        /// <summary>
        /// Return the path to the IgClient binary
        /// </summary>
        private string IgClientPath()
        {
            var path = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86);
            if (path == "") path = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles);

            if (File.Exists(path + "/IguanaIR/igclient.exe")) return path + "/IguanaIR/igclient.exe";

            return "igclient"; // Hope it's on the path...
        }
    }
}
