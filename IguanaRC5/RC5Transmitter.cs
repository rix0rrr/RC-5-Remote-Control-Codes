using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IguanaRC5
{
    /// <summary>
    /// Facade class for the transmitter library
    /// </summary>
    public class RC5Transmitter
    {
        private IguanaClient client = new IguanaClient();
        private RC5Encoder encoder  = new RC5Encoder(new RegistryRC5EncoderState());

        /// <summary>
        /// Initialize the client with the correct carrier frequency
        /// </summary>
        public void Initialize()
        {
            client.SetCarrierFrequency(RC5Encoder.CarrierFrequency);
        }

        /// <summary>
        /// Transmit an RC5 word according to the repeat count inside
        /// </summary>
        public void Transmit(RC5Word word)
        {
            encoder.EncodeWord(word).Each(client.TransmitSequence);
        }

        /// <summary>
        /// Transmit all RC5 Words in the sequence
        /// </summary>
        public void TransmitAll(IEnumerable<RC5Word> words)
        {
            words.Each(Transmit);
        }

        /// <summary>
        /// Transmit all RC5 Words in the sequence
        /// </summary>
        public void TransmitAll(params RC5Word[] words)
        {
            words.Each(Transmit);
        }
    }
}
