using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IguanaRC5
{
    internal static class LinqExtensions
    {
        /// <summary>
        /// Perform an action for each element in a sequence
        /// </summary>
        public static void Each<T>(this IEnumerable<T> coll, Action<T> action)
        {
            foreach (var el in coll)
            {
                action(el);
            }
        }

        /// <summary>
        /// Intersperse an element into a collection
        /// </summary>
        public static IEnumerable<T> Intersperse<T>(this IEnumerable<T> source, T element)
        {
            using (var i = source.GetEnumerator())
            {
                if (i.MoveNext()) yield return i.Current;

                while (i.MoveNext())
                {
                    yield return element;
                    yield return i.Current;
                }
            }
        }

        /// <summary>
        /// ToString() the connection
        /// </summary>
        public static string CollToString<T>(this IEnumerable<T> source)
        {
            return string.Join(", ", source.Select(_ => _.ToString()));
        }

        /// <summary>
        /// Group elements of the sequence by consecutive matching values of the selector function
        /// </summary>
        public static IEnumerable<Tuple<U, IEnumerable<T>>> SequentialGroupBy<T, U>(this IEnumerable<T> source, Func<T, U> selector)
        {
            using (var i = source.GetEnumerator())
            {
                if (i.MoveNext()) 
                {
                    var val = selector(i.Current);
                    var acc = new List<T>() { i.Current };
    
                    while (i.MoveNext())
                    {
                        if (!selector(i.Current).Equals(val))
                        {
                            yield return Tuple.Create(val, (IEnumerable<T>)acc);
                            acc.Clear();
                            val = selector(i.Current);
                        }
                        acc.Add(i.Current);
                    }

                    yield return Tuple.Create(val, (IEnumerable<T>)acc);
                }
            }
        }
    }
}
