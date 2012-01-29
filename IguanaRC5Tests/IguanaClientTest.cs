using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using IguanaRC5;

namespace IguanaRC5Tests
{
    class IguanaClientTest
    {
        private IguanaClient client;

        [SetUp]
        public void SetUp()
        {
            client = new IguanaClient();
            client.SetCarrierFrequency(36000);
        }

        [Test]
        public void TransmitNormalSequence()
        {
            client.TransmitSequence(new Segment[]
            {
                new Segment(true, 889),
                new Segment(false, 889)
            });
        }

        [Test]
        public void TransmitSequenceStartingWithSleep()
        {
            client.TransmitSequence(new Segment[]
            {
                new Segment(false, 889),
                new Segment(true, 889)
            });
        }

        [Test]
        public void TransmitSequenceWithRepeats()
        {
            client.TransmitSequence(new Segment[]
            {
                new Segment(true, 889),
                new Segment(false, 889),
                new Segment(false, 889),
                new Segment(true, 889)
            });
        }
    }
}
