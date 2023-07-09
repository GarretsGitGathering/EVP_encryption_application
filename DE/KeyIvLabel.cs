using System;
using System.IO;
using System.Text;
using Pango;
using Cairo;
namespace DE
{
    [System.ComponentModel.ToolboxItem(true)]
    public class ShowKeyIv : Gtk.DrawingArea
    {
        Pango.Layout layout;

        public ShowKeyIv()
        {
            // Insert initialization code here.
        }
        protected override bool OnButtonPressEvent(Gdk.EventButton ev)
        {
            // Insert button press handling code here.
            return base.OnButtonPressEvent(ev);
        }
        protected override bool OnExposeEvent(Gdk.EventExpose ev)
        {
            base.OnExposeEvent(ev);
            // Insert drawing code here.
            this.GdkWindow.DrawLayout(this.Style.TextGC(this.State), 0, 0, layout);
            return true;
        }
        protected override void OnSizeAllocated(Gdk.Rectangle allocation)
        {
            base.OnSizeAllocated(allocation);
            // Insert layout code here.
            layout = new Pango.Layout(this.PangoContext);
            layout.Wrap = Pango.WrapMode.Word;
            layout.FontDescription = FontDescription.FromString("Tahoma 12");
            layout.SetMarkup(" files encrypted!\n\n "+ DE.MainClass.Return_key_and_iv("/home/user/Documents/Projects/c encryptor/target2/"));
        }
        protected override void OnSizeRequested(ref Gtk.Requisition requisition)
        {
            // Calculate desired size here.
            requisition.Height = 25;
            requisition.Width = 20;
        }
    }
}
