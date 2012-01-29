using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace IguanaRC5
{
    /// <summary>
    /// Representation of an RC5 code word
    /// </summary>
    public struct RC5Word
    {
        private static Regex ReprRegex = new Regex(@"^(\d+):(\d+)(?:x(\d+))?$");

        public RC5Word(int system, int command, int count=1) : this()
        {
            System  = system;
            Command = command;
            Count   = count;
        }

        /// <summary>
        /// The system number (0..63)
        /// </summary>
        public int System { get; private set; }

        /// <summary>
        /// The command number (0..127)
        /// </summary>
        public int Command { get; private set; }

        /// <summary>
        /// The repeat number (1..)
        /// </summary>
        public int Count { get; private set; }

        /// <summary>
        /// Return an RC5Word that represents the current code (and repeat count)
        /// repeated some more times times.
        /// </summary>
        public RC5Word Repeat(int repeats)
        {
            return new RC5Word(System, Command, Count * repeats);
        }

        /// <summary>
        /// Return a string representation of the code word
        /// </summary>
        /// <remarks>
        /// Formatted as follows:
        /// 
        ///  SYSTEM ':' COMMAND [ 'x' COUNT ]
        /// </remarks>
        public override string ToString()
        {
            return string.Format("{0}:{1}{2}",
                System,
                Command,
                Count != 1 ? ("x" + Count) : "");
        }

        /// <summary>
        /// Parse an RC5 word from the string representation
        /// </summary>
        public static RC5Word Parse(string repr)
        {
            var m = ReprRegex.Match(repr);
            if (!m.Success) throw new ArgumentException(string.Format("Unable to parse RC5 word: {0}", repr));

            return new RC5Word(
                int.Parse(m.Groups[1].ToString()),
                int.Parse(m.Groups[2].ToString()),
                m.Groups.Count > 3 ? int.Parse(m.Groups[3].ToString()) : 1);
        }
    }
}
