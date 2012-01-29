using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IguanaRC5
{
    /// <summary>
    /// Class for encoding RC5 words into a sequence of pulses and spaces
    /// </summary>
    internal class RC5Encoder
    {
        public const int CarrierFrequency = 36000;

        const int HalfBitDurationUsec = 889;
        const int RepeatPauseUsec     = 100000;

        private readonly IRC5EncoderState state;

        public RC5Encoder(IRC5EncoderState state)
        {
            this.state = state;
        }

        /// <summary>
        /// Encode a word, along with it repeats, into a sequence of segments
        /// </summary>
        /// <remarks>
        /// Because the IguanaWorks client has a built-in timeout that 
        /// can't deal with sequences that are too long, every repeat
        /// will have its own sequence.
        /// </remarks>
        public IEnumerable<IEnumerable<Segment>> EncodeWord(RC5Word word)
        {
            var result =
                Enumerable.Range(0, word.Count)
                .SelectMany(_ => new IEnumerable<Segment>[] {
                    EncodeWithoutRepeat(word),
                    // Have a word sleep after every word, so we can arbitrarily append
                    // encoded words
                    new Segment[] { new Segment(false, RepeatPauseUsec) }
                    });

            state.ToggleBit = !state.ToggleBit;

            return result;
        }

        /// <summary>
        /// Encode a single RC5 Word into a sequence of segments, ignoring repeats
        /// </summary>
        private IEnumerable<Segment> EncodeWithoutRepeat(RC5Word word)
        {
            return WordBits(word).SelectMany(BitToTransition);
        }

        /// <summary>
        /// Return the transition that needs to be encoded to transmit a given bit
        /// </summary>
        private IEnumerable<Segment> BitToTransition(bool bit)
        {
            yield return new Segment(!bit, HalfBitDurationUsec);
            yield return new Segment(bit, HalfBitDurationUsec);
        }

        /// <summary>
        /// Return the bits that need to be encoded for a word
        /// </summary>
        private IEnumerable<bool> WordBits(RC5Word word)
        {
            return new bool[] {
                true,
                (word.Command & 0x40) == 0, // Note: bit inverted
                state.ToggleBit,
                // System
                (word.System & 0x10) != 0,
                (word.System & 0x08) != 0,
                (word.System & 0x04) != 0,
                (word.System & 0x02) != 0,
                (word.System & 0x01) != 0,
                // Command
                (word.Command & 0x20) != 0,
                (word.Command & 0x10) != 0,
                (word.Command & 0x08) != 0,
                (word.Command & 0x04) != 0,
                (word.Command & 0x02) != 0,
                (word.Command & 0x01) != 0
            };
        }
    }
}
