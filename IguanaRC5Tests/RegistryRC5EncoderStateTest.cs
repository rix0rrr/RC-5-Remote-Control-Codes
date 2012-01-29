using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using IguanaRC5;

namespace IguanaRC5Tests
{
    class RegistryRC5EncoderStateTest
    {
        [Test]
        public void WritingAndReading()
        {
            var state = new RegistryRC5EncoderState();

            var canRead = state.ToggleBit;

            state.ToggleBit = true;
            Assert.True(state.ToggleBit);

            state.ToggleBit = false;
            Assert.False(state.ToggleBit);
        }
    }
}
