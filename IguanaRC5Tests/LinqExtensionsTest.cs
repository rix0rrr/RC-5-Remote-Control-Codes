using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using IguanaRC5;

namespace IguanaRC5Tests
{
    class LinqExtensionsTest
    {
        [Test]
        public void GroupByEvenness()
        {
            var input = new int[] {
                1, 2, 4, 5, 7, 8
            };

            var grouped = input.SequentialGroupBy(_ => _ % 2 == 0).Select(_ => _.Item2.ToList()).ToList();

            CollectionAssert.AreEquivalent(new int[][] {
                new int[] { 1 },
                new int[] { 2, 4 },
                new int[] { 5, 7 },
                new int[] { 8 },
            }, grouped);
        }
    }
}
