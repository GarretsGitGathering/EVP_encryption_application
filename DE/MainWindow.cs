using System;
using Gtk;

public partial class MainWindow : Gtk.Window
{
    public MainWindow() : base(Gtk.WindowType.Toplevel)
    {
        Build();
    }

    protected void OnDeleteEvent(object sender, DeleteEventArgs a)
    {
        Application.Quit();
        a.RetVal = true;
    }

    protected void onButtonPress(object sender, EventArgs e)
    {
        if (this.direct_entry.Text != "" && this.ext_entry.Text != "")
        {
            DE.MainClass.Encryptor(this.direct_entry.Text, this.ext_entry.Text);
            Console.WriteLine("");
            Window tmp = new DE.KeyWindow();
            tmp.Show();
        }
        else
        {
            Console.WriteLine("directory or extension not inserted");
        }
    }

    protected void OnDirectEntered(object o, TextInsertedArgs args)
    {
        //Console.WriteLine(this.direct_entry.Text);
    }

    protected void OnExtEntered(object o, TextInsertedArgs args)
    {
        //Console.WriteLine(this.ext_entry.Text);
    }
}
