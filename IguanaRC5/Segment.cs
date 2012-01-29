using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IguanaRC5
{
    /// <summary>
    /// Representation of the state of the IR LED for a duration of time
    /// </summary>
    internal struct Segment
    {
        public Segment(bool on, int microseconds) : this()
        {
            On = on;
            Microseconds = microseconds;
        }

        /// <summary>
        /// Whether the LED is on for this segment
        /// </summary>
        public bool On { get; private set; }

        /// <summary>
        /// The duration of the segment
        /// </summary>
        public int Microseconds { get; private set; }

        public override string ToString()
        {
            if (On)
                return string.Format("\u203E{0}\u203E", Microseconds);
            else
                return string.Format("_{0}_", Microseconds);
        }
    }
}
