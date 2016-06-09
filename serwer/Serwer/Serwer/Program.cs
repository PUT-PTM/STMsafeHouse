using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;

namespace System
{
    class Program
    {
        static void Main(string[] args)
        {
            //zanicjowanie serwera
            TcpListener Serwer = new TcpListener(IPAddress.Parse("0.0.0.0"), 7777);
            Serwer.Start();

            //utworzenie bufora służacego do odczytywania wiadomości
            Byte[] bytes = new Byte[256];
            String data = null;

        
            //czekanie na klienta
            Console.WriteLine("Czekam na połączenie... ");
            TcpClient client = Serwer.AcceptTcpClient();
            Console.WriteLine("Połączono.");

            //inicjalizacja zmiennych pomocniczych
            NetworkStream stream = client.GetStream();
            int i; //służy do zliczania przesyłanych bajtów

            //wejście do pętli czytania komunikatów
            do
            {
                //czytanie odpowiedzi - strumień bajtów jest dekodowany i zapisany do zmiennej "data"
                i = stream.Read(bytes, 0, bytes.Length);
                data = System.Text.Encoding.ASCII.GetString(bytes, 0, i);
                Console.WriteLine("Odpowiedz: " + data);

                if (data == "sendmail")
                {
                    run_cmd("myemail.py");
                }
                  
            } while (true);

            //zamknięcie komunikacji
            stream.Close();
            Serwer.Stop();

        }

        static void run_cmd(string cmd)
        {
            System.Diagnostics.ProcessStartInfo start = new System.Diagnostics.ProcessStartInfo();
            start.FileName = "C:/Users/Dominika/Desktop/myemail.py";
            start.Arguments = string.Format("{0} {1}", cmd, "");
            start.UseShellExecute = true;
            System.Diagnostics.Process proces = System.Diagnostics.Process.Start(start);
        }
    }
}
