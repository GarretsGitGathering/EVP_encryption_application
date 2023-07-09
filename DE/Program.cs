using System;
using Gtk;
using System.Runtime.CompilerServices;

namespace DE
{
    class MainClass
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Encryptor(string directory, string ext);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static string Return_key_and_iv(string file_contents);

        public static void Main(string[] args)
        {
            Application.Init();
            MainWindow win = new MainWindow();
            win.Show();
            Application.Run();
        }
    }
}