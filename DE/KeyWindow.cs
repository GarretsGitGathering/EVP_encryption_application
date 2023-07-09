using System;
using Gtk;
namespace DE
{
    public partial class KeyWindow : Gtk.Window
    {
        public KeyWindow() : base(Gtk.WindowType.Toplevel)
        {
            this.Build();
        }
    }
}
