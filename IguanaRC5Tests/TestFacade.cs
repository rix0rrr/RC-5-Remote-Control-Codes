using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using IguanaRC5;
using NUnit.Framework;

namespace IguanaRC5Tests
{
    class TestFacade
    {
        [Test]
        public void Script()
        {
            var trans = new RC5Transmitter();
            trans.TransmitAll(
                TVCodes.SourcesMenu.Repeat(2),
                TVCodes.ArrowDown,
                TVCodes.ArrowDown,
                TVCodes.ArrowDown,
                TVCodes.Confirm);
        }
    }
}
