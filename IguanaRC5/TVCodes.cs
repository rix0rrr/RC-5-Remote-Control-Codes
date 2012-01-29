using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IguanaRC5
{
    /// <summary>
    /// Some codes that work for some RC5 enabled TVs (no guarantees!)
    /// </summary>
    public static class TVCodes
    {
        public static RC5Word Ch0 = new RC5Word(0, 0);
        public static RC5Word Ch1 = new RC5Word(0, 1);
        public static RC5Word Ch2 = new RC5Word(0, 2);
        public static RC5Word Ch3 = new RC5Word(0, 3);
        public static RC5Word Ch4 = new RC5Word(0, 4);
        public static RC5Word Ch5 = new RC5Word(0, 5);
        public static RC5Word Ch6 = new RC5Word(0, 6);
        public static RC5Word Ch7 = new RC5Word(0, 7);
        public static RC5Word Ch8 = new RC5Word(0, 8);
        public static RC5Word Ch9 = new RC5Word(0, 9);

        public static RC5Word VolUp   = new RC5Word(0, 16);
        public static RC5Word VolDown = new RC5Word(0, 17);

        public static RC5Word ArrowLeft  = new RC5Word(0, 85);
        public static RC5Word ArrowRight = new RC5Word(0, 86);
        public static RC5Word ArrowUp    = new RC5Word(0, 80);
        public static RC5Word ArrowDown  = new RC5Word(0, 81);

        public static RC5Word SourcesMenu = new RC5Word(0, 56);
        public static RC5Word Confirm     = new RC5Word(0, 87);

        public static RC5Word PowerOff    = new RC5Word(0, 12);
    }
}
