using System;
using Pango;
using Cairo;
namespace DE
{
    [System.ComponentModel.ToolboxItem(true)]
    public class DrawingArea : Gtk.DrawingArea
    {
        Pango.Layout layout;

        public DrawingArea()
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
            layout.SetMarkup("Insert a directory to encrypt");
        }
        protected override void OnSizeRequested(ref Gtk.Requisition requisition)
        {
            // Calculate desired size here.
            requisition.Height = 25;
            requisition.Width = 20;
        }
    }
}
