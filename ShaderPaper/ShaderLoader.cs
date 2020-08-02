using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Newtonsoft.Json;

namespace ShaderPaper
{
    public struct ShaderPaper
    {
        public string path;
        public string name;
        public string author;

        public string vertShader;
        public string fragShader;

        public BitmapImage imgPreview;

        public BitmapImage imgChannel0;
        public BitmapImage imgChannel1;
        public BitmapImage imgChannel2;
        public BitmapImage imgChannel3;
    }

    public struct ShaderPaperConfig
    {
        public string name;
        public string author;
        public string preview;
    }

    public static class ShaderLoader
    {
        private static readonly string ShaderPaperPath = Directory.GetCurrentDirectory() + @"\shaderpapers";

        private static Process shaderPaperDProcess;

        public static List<ShaderPaper> GetShaderPapers()
        {
            var shaderPapers = new List<ShaderPaper>();

            var shaderPaperDirectories = Directory.GetDirectories(ShaderPaperPath);
            foreach (var folder in shaderPaperDirectories)
            {
                string shaderPaperConfigFile = folder + @"\shaderpaper.json";
                if (!File.Exists(shaderPaperConfigFile))
                    continue;

                var parsedConfig = JsonConvert.DeserializeObject<ShaderPaperConfig>(File.ReadAllText(shaderPaperConfigFile));

                var shaderPaper = new ShaderPaper();
                shaderPaper.name = parsedConfig.name;
                shaderPaper.author = parsedConfig.author;
                shaderPaper.path = folder;

                try { shaderPaper.vertShader = File.ReadAllText(folder + @"\vert.glsl"); } catch { continue; }
                try { shaderPaper.fragShader = File.ReadAllText(folder + @"\frag.glsl"); } catch { continue; }

                try { shaderPaper.imgPreview = new BitmapImage(new Uri(folder + @"\" + parsedConfig.preview)); } catch { }
                try { shaderPaper.imgChannel0 = new BitmapImage(new Uri(folder + @"\channel0.png")); } catch { }
                try { shaderPaper.imgChannel1 = new BitmapImage(new Uri(folder + @"\channel1.png")); } catch { }
                try { shaderPaper.imgChannel2 = new BitmapImage(new Uri(folder + @"\channel2.png")); } catch { }
                try { shaderPaper.imgChannel3 = new BitmapImage(new Uri(folder + @"\channel3.png")); } catch { }


                shaderPapers.Add(shaderPaper);
            }

            return shaderPapers;
        }

        public static void StartDaemon(string shaderPaperPath)
        {
            try
            {
                shaderPaperDProcess = Process.Start("ShaderPaperD.exe", "\"" + shaderPaperPath + "\"");
            } catch
            {

            }
        }

        public static void StopDaemon()
        {
            shaderPaperDProcess?.Kill();
            shaderPaperDProcess = null;
        }

        public static bool IsDeamonRunning()
        {
            return Process.GetProcessesByName("ShaderPaperD").Length > 0;
        }
    }
}
