/* AUTORES
 * Carlos Ernesto Soto Alarcón A01747990
 * Sergio Alfonso Casillas Santoyo A01424863
 * Karime Itzel Ruvalcaba Pérez A01656188
 */
 #include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class ActInt1 {
public:
    /*
    * Función auxiliar para leer las líneas del archivo.
    * Complejidad: O(n), n es el número de líneas en el archivo.
    */
    static vector<string> abrir_achivo_p1(const string& file_name) {
        vector<string> lines;
        ifstream file(file_name);
        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
        return lines;
    }

    /**
    * Función auxiliar para leer un archivo como un solo string.
    * Complejidad: O(n), n es el número de caracteres en el archivo.
    */
    static string abrir_achivo_p2_p3(const string& file_name) {
        ifstream file(file_name);
        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
        return content;
    }

    /**
    * Algoritmo de Knuth-Morris-Pratt (KMP) para la búsqueda de substrings.
    * Complejidad: O(m + n), m es la longitud del "needle" y "n" es la longitud "haystack".
     */
    int kmp(const string& haystack, const string& needle) {
        if (needle.empty()) return 0;

        vector<int> lps(needle.size(), 0);
        int j = 0;
        for (int i = 1; i < needle.size(); ++i) {
            if (needle[i] == needle[j]) {
                lps[i] = ++j;
            } else if (j > 0) {
                j = lps[j - 1];
                --i;
            }
        }

        j = 0;
        for (int i = 0; i < haystack.size(); ++i) {
            if (haystack[i] == needle[j]) {
                if (++j == needle.size()) {
                    return i - j + 1;
                }
            } else if (j > 0) {
                j = lps[j - 1];
                --i;
            }
        }
        return -1;
    }

    /**
    * Parte 1: Detectar secuencias de chars maliciosas.
    * Complejidad:
    * - O(t * m * (h + n)), t es el número de archivos de transmisión, m es el número de archivos maliciosos
    * h es el tamaño promedio del "haystack" y n es del tamaño promedio de "needle"
     */
    string parte_1(const vector<string>& transmission_files, const vector<string>& mcode_files) {
        string result;
        for (const auto& transmission_file : transmission_files) {
            for (const auto& mcode_file : mcode_files) {
                vector<string> needles = abrir_achivo_p1(mcode_file);
                string haystack = abrir_achivo_p2_p3(transmission_file);

                bool found = false;
                for (const auto& needle : needles) {
                    int index = kmp(haystack, needle);
                    if (index != -1) {
                        result += "true " + to_string(index + 1) + "\n";
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    result += "false\n";
                }
            }
        }
        return result;
    }

    /**
    * Algoritmo de Manacher para encontrar el palíndromo más largo.
    * Complejidad: O(n), n es la longitud del texto.
     */
    pair<int, int> manacher(const string& text) {
        int n = text.size();
        vector<int> lps(2 * n + 1, 0);
        int c = 0, r = 0, maxLength = 0, center = 0;

        for (int i = 0; i < 2 * n + 1; ++i) {
            int mirror = 2 * c - i;
            if (i < r) {
                lps[i] = min(r - i, lps[mirror]);
            }

            int a = i + lps[i] + 1;
            int b = i - lps[i] - 1;
            while (a < 2 * n + 1 && b >= 0 && (a % 2 == 0 || text[a / 2] == text[b / 2])) {
                ++lps[i];
                ++a;
                --b;
            }

            if (i + lps[i] > r) {
                c = i;
                r = i + lps[i];
            }

            if (lps[i] > maxLength) {
                maxLength = lps[i];
                center = i;
            }
        }

        int start = (center - maxLength) / 2;
        return {start + 1, start + maxLength};
    }

    /**
    * Parte 2: Encontrar el palíndromo de chars más largo en el archivo de transmisión
    * Complejidad: O(n), n es la longitud del texto en el archivo de transmisión
     */
    string parte_2(const string& trans) {
        string transmission = abrir_achivo_p2_p3(trans);
        auto [start, end] = manacher(transmission);
        string palindrome = transmission.substr(start - 1, end - start + 1);
        return to_string(start) + " " + to_string(end) + " " + palindrome;
    }

    /**
    * Algoritmo de programación dinámica para encontrar el substring común más largo.
    * Complejidad: O(m * n), m y n son las longitudes de los dos strings de entrada.
     */
    pair<int, int> dp(const string& s1, const string& s2) {
        int m = s1.size(), n = s2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        int maxLength = 0, endIndex = 0;

        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                    if (dp[i][j] > maxLength) {
                        maxLength = dp[i][j];
                        endIndex = i;
                    }
                }
            }
        }

        int start = endIndex - maxLength;
        return {start + 1, endIndex};
    }

    /**
    * Parte 3: Encontrar el substring común más largo entre los archivos de transmisión
    * Complejidad: O(m * n), m y n son las longitudes del contenido de los dos archivos
     */
    string parte_3(const string& s1, const string& s2) {
        string string1 = abrir_achivo_p2_p3(s1);
        string string2 = abrir_achivo_p2_p3(s2);
        auto [start, end] = dp(string1, string2);
        string common_substring = string1.substr(start - 1, end - start + 1);
        return to_string(start) + " " + to_string(end) + " " + common_substring;
    }

    /**
    * Función principal para ejecutar las tres partes del programa.
    *
     */
    void main() {
        vector<string> transmission_files = {"transmission1.txt", "transmission2.txt"};
        vector<string> mcode_files = {"mcode1.txt", "mcode2.txt", "mcode3.txt"};

        // Parte 1
        cout << "\nParte 1:\n\n";
        cout << parte_1(transmission_files, mcode_files);

        // Parte 2
        cout << "\nParte 2:\n\n";
        cout << parte_2("transmission1.txt") << endl;
        cout << parte_2("transmission2.txt") << endl;

        // Parte 3
        cout << "\nParte 3:\n\n";
        cout << parte_3("transmission1.txt", "transmission2.txt") << endl;
    }
};

int main() {
    ActInt1 solution;
    solution.main();
    return 0;
}