using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ShaderPaper
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private List<ShaderPaper> ShaderPapers;
        private int CurrentShaderPaper = -1;

        private System.Windows.Forms.NotifyIcon TrayIcon;

        public MainWindow()
        {
            InitializeComponent();

            this.TrayIcon = new System.Windows.Forms.NotifyIcon();
            this.TrayIcon.Text = "ShaderPaper";
            this.TrayIcon.Visible = false;
            this.TrayIcon.Icon = Properties.Resources.TrayIcon;
            this.TrayIcon.DoubleClick += (s, e) =>
            {
                this.TrayIcon.Visible = false;
                this.Show();
                this.WindowState = WindowState.Normal;
            };

            this.Closing += (s, e) => ShaderLoader.StopDaemon();
            this.StateChanged += (s, e) =>
            {
                if (this.WindowState == WindowState.Minimized)
                {
                    this.Hide();
                    this.TrayIcon.Visible = true;
                }
            };

            this.ShaderPapers = ShaderLoader.GetShaderPapers();

            foreach (var shaderPaper in this.ShaderPapers)
                this.cboxShaderPapers.Items.Add(shaderPaper.name);

        }

        private void cboxShaderPapers_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var cbox = (sender as ComboBox);
            if (cbox.SelectedIndex < 0)
            {
                this.lblName.Content = "";
                this.lblAuthor.Content = "";
                this.imgPreview.Source = null;

                this.imgChannel0.Source = null;
                this.imgChannel1.Source = null;
                this.imgChannel2.Source = null;
                this.imgChannel3.Source = null;

                this.txtVertexShader.Text = "";
                this.txtFragmentShader.Text = "";

                this.btnUse.IsEnabled = false;
                return;
            }

            var selectedShaderPaper = this.ShaderPapers[cbox.SelectedIndex];

            this.lblName.Content = selectedShaderPaper.name;
            this.lblAuthor.Content = selectedShaderPaper.author;
            this.imgPreview.Source = selectedShaderPaper.imgPreview;

            this.imgChannel0.Source = selectedShaderPaper.imgChannel0;
            this.imgChannel1.Source = selectedShaderPaper.imgChannel1;
            this.imgChannel2.Source = selectedShaderPaper.imgChannel2;
            this.imgChannel3.Source = selectedShaderPaper.imgChannel3;

            this.txtVertexShader.Text = selectedShaderPaper.vertShader;
            this.txtFragmentShader.Text = selectedShaderPaper.fragShader;

            this.btnUse.IsEnabled = true;

            if (this.CurrentShaderPaper == cbox.SelectedIndex)
                this.btnUse.Content = "Stop";
            else
                this.btnUse.Content = "Use";
        }

        private void btnUse_Click(object sender, RoutedEventArgs e)
        {
            if (ShaderLoader.IsDeamonRunning())
                ShaderLoader.StopDaemon();

            if (this.CurrentShaderPaper == this.cboxShaderPapers.SelectedIndex)
            {
                this.btnUse.Content = "Use";

                this.CurrentShaderPaper = -1;
            }
            else
            {
                this.btnUse.Content = "Stop";

                this.CurrentShaderPaper = this.cboxShaderPapers.SelectedIndex;
                ShaderLoader.StartDaemon(this.ShaderPapers[this.CurrentShaderPaper].path);
            }

        }
    }
}
