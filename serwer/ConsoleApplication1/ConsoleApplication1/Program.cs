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


                //jeśli otrzymano wiadomość "close", wysłanie potwierdzenia
                if (data == "close")
                {
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes("ack");
                    stream.Write(msg, 0, msg.Length);
                }

                if (data == "sendmail")
                {
                    run_cmd("myemail.py");
                }

                //jeśli nie - serwer pyta użytkownika, czy zakończyć połączenie, czy czekać na kolejną wiadomość
                else
                {
                    Console.WriteLine("Czy chcesz zakonczyc polaczenie? (tak/nie)");

                    //odpowiedź użytkownika jest zapamiętana w zmienej "finish"
                    String finish = Console.ReadLine();


                    //jeśli tak, to serwer wysyła odpowiedź "close" i czeka na potwierdzenie
                    if (finish == "tak")
                    {
                        do
                        {
                            byte[] msg = System.Text.Encoding.ASCII.GetBytes("abcdefghijklmno\n");
                            stream.Write(msg, 0, msg.Length);
                        } while (Console.ReadLine() == "k");
                        i = stream.Read(bytes, 0, bytes.Length);
                        data = System.Text.Encoding.ASCII.GetString(bytes, 0, i);

                        Console.WriteLine("Odpowiedz: " + data);

                        //do zmiennej "data" jest wpisana wartość "close", aby wyjść z pętli
                        data = "close";
                    }

                    //jeśli nie, to serwer wysyła odpowiedź "nope" i wraca do początku pętli
                    else
                    {
                        byte[] msg = System.Text.Encoding.ASCII.GetBytes("nope");
                        stream.Write(msg, 0, msg.Length);
                    }

                }
            } while (data != "close");
            //pętla wykonuje się, aż zmienna "data" nie przyjmie wartości "close"

            //zamknięcie komunikacji
            stream.Close();
            Serwer.Stop();

            //aplikacja czeka, aż użytkownik naciśnie enter przed zakończeniem działania
            Console.WriteLine("\nNaciśnij enter, aby wyjść.");
            Console.Read();
        }

        static void run_cmd(string cmd)
        {
            System.Diagnostics.ProcessStartInfo start = new System.Diagnostics.ProcessStartInfo();
            start.FileName = "C:/Users/Dominika/Desktop/myemail.py";
            start.Arguments = string.Format("{0} {1}", cmd, "");
            start.UseShellExecute = true;
            // start.UseShellExecute = false;
            // start.RedirectStandardOutput = true;
            System.Diagnostics.Process proces = System.Diagnostics.Process.Start(start);
          /*  using (System.Diagnostics.Process process = System.Diagnostics.Process.Start(start))
            {
                using (StreamReader reader = process.StandardOutput)
                {
                    string result = reader.ReadToEnd();
                    Console.Write(result);
                }
            } */
        }
    }  
}
