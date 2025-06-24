using System;
using System.Linq;
using System.IO;
using System.Globalization;

namespace MACSLangRuntime
{
    public static class Program
    {
        public static int fatorial(int n)
        {
int resultado = 1            ;
for (int i = 1; i <= n; i = i + 1            )
            {
resultado = resultado * i                ;
            }
return resultado            ;
        }
        
        public static int Main(string[] args)
        {
            Console.SetOut(new StreamWriter(Console.OpenStandardOutput()) { AutoFlush = true });
            {
Console.WriteLine("Digite um número para calcular o fatorial:"                );
int numero = 0                ;
                Console.Out.Flush();
numero = int.Parse(Console.ReadLine()!, CultureInfo.InvariantCulture);                
int fat = fatorial(numero)                ;
Console.WriteLine("O fatorial de " + numero + " é " + fat                );
float myFloat = 123.45f                ;
char myChar = 'X'                ;
bool myBool = true                ;
if (myBool == false)                 {
                }
else                 {
int temp = 0                    ;
                }
while (myBool)                 {
myBool = false                    ;
                }
bool exprResult = 5 + 3 * 2 == 11 && !false || true                ;
return 0                ;
            }
        }
        
    }
}
