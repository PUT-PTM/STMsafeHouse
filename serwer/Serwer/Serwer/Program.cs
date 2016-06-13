using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;

namespace System
{
    class Program
    {
        //tablice loginów i haseł
        public static String[] loginy = { "DominikaP", "DominikaC", "Tomek" };
        public static String[] hasla = { "1234", "8888", "0000" };

        static void Main(string[] args)
        {
            //zanicjowanie serwera
            TcpListener Serwer = new TcpListener(IPAddress.Parse("0.0.0.0"), 7777);
            Serwer.Start();

            Thread thr = new Thread(localListener);
            thr.Start();

            //utworzenie bufora służacego do odczytywania wiadomości
            Byte[] bytes = new Byte[256];
            String data = null;

            //utworzenie bufora służącego do wysyłnia wiadomości
            Byte[] bytes2 = new Byte[256];
            String data2 = null;

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
                else
                {
                    if (data == "passwd")
                    {
                        Console.WriteLine("Podaj nazwe uzytkownika: ");
                        String login = Console.ReadLine();
                        Console.WriteLine("Podaj stare haslo: ");
                        String stare_haslo = Console.ReadLine();
                        if (check_pass_and_login(login, loginy, stare_haslo, hasla) == true)
                        {
                            Console.WriteLine("Podaj nowe haslo: ");
                            String nowe_haslo = Console.ReadLine();
                            Console.WriteLine("Powtorz nowe haslo: ");
                            String nowe_haslo2 = Console.ReadLine();
                            if (nowe_haslo == nowe_haslo2)
                            {
                                for (int j = 0; j < loginy.Length; j++)
                                {
                                    if (login == loginy[j]) hasla[j] = nowe_haslo;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (check_pass(data, hasla) == true) data2 = "verification:passed";
                        else data2 = "verification:failed";
                        stream.Write(Encoding.Default.GetBytes(data2), 0, data2.Length);
                        stream.Flush();
                    }
                   
                }
                  
            } while (true);

            //zamknięcie komunikacji
            stream.Close();
            Serwer.Stop();
        }

        static void localListener()
        {
            while (true)
            {
                Thread.Sleep(1000);
                String data = Console.ReadLine();
                if (data == "passwd")
                {
                    Console.WriteLine("Podaj nazwe uzytkownika: ");
                    String login = Console.ReadLine();
                    Console.WriteLine("Podaj stare haslo: ");
                    String stare_haslo = Console.ReadLine();
                    if (check_pass_and_login(login, loginy, stare_haslo, hasla) == true)
                    {
                        Console.WriteLine("Podaj nowe haslo(4 cyfry): ");
                        String nowe_haslo = Console.ReadLine();
                        Console.WriteLine("Powtorz nowe haslo: ");
                        String nowe_haslo2 = Console.ReadLine();
                        if (nowe_haslo == nowe_haslo2 && nowe_haslo.Length == 4)
                        {
                            for (int j = 0; j < loginy.Length; j++)
                            {
                                if (login == loginy[j])
                                {
                                    hasla[j] = nowe_haslo;
                                    Console.WriteLine("Haslo zostalo zmienione");
                                }   
                            }
                        }else Console.WriteLine("Podane hasla sa bledne"); 
                    }
                    else Console.WriteLine("Podano zly login lub haslo");
                }
            }
        }

        static bool check_pass(String data, String[] hasla)
        {
            for (int i = 0; i < hasla.Length; i++)
            {
                if (data == hasla[i]) return true;
            }
                    return false;
        }

        static bool check_pass_and_login(String login, String[] loginy, String pass, String[] hasla)
        {
            for(int i=0; i<loginy.Length; i++)
            {
                if (login == loginy[i] && pass == hasla[i]) return true;
            }
            return false;
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
