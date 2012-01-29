using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IguanaRC5
{
    /// <summary>
    /// Class representating a temporary file
    /// </summary>
    /// <remarks>
    /// Class graciously donated by Marc Gravell at StackOverflow ;)
    /// </remarks>
    internal class TempFile : IDisposable
    {
        string path;

        public TempFile() : this(System.IO.Path.GetTempFileName()) { }

        public TempFile(string path)
        {
            if (string.IsNullOrEmpty(path)) throw new ArgumentNullException("Path is empty");
            this.path = path;
        }

        public string Path
        {
            get
            {
                if (path == null) throw new ObjectDisposedException(GetType().Name);
                return path;
            }
        }

        public void Dispose() 
        {
            if (path != null)
            {
                try { File.Delete(path); }
                catch { } // best effort
                path = null;
            }
        }
    }
}
