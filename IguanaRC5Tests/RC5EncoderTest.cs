using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using IguanaRC5;
using Moq;

namespace IguanaRC5Tests
{
    class RC5EncoderTest
    {
        private Mock<IRC5EncoderState> state;
        private RC5Encoder encoder;

        [SetUp]
        public void SetUp()
        {
            state   = new Mock<IRC5EncoderState>();
            encoder = new RC5Encoder(state.Object);

            state.SetupGet(_ => _.ToggleBit).Returns(false);
        }

        [Test]
        public void EncodeTogglesState()
        {
            encoder.EncodeWord(new RC5Word(0, 0));
            state.VerifySet(_ => _.ToggleBit = true);
        }

        [Test]
        public void EncodingWord()
        {
            var e = encoder.EncodeWord(new RC5Word(0, 1)).Select(_ => _.ToList()).ToList();

            var one  = new Segment[] { new Segment(false, 889), new Segment(true, 889) };
            var zero = new Segment[] { new Segment(true, 889), new Segment(false, 889) };

            Assert.AreEqual(1, e.Count());
            CollectionAssert.AreEquivalent(new IEnumerable<Segment>[] {
                one,
                one,  // Low command
                zero, // Toggle bit
                zero, // System (5 bits)
                zero,
                zero,
                zero,
                zero,
                zero, // Command (6 bits)
                zero,
                zero,
                zero,
                zero,
                one
            }.SelectMany(x => x).ToList(), e.ElementAt(0));
        }
    }
}
