/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.tpid;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 *
 * @author Pedro Pereira
 */
public class WrappersTP {
    static String obtem_nome_pais(String pesquisa) throws FileNotFoundException, IOException {
        // Link para a página da Wikipédia
        String link = "https://en.wikipedia.org/wiki/";
        
        // Realiza a requisição HTTP e salva o conteúdo em um arquivo local
        HttpRequestFunctions.httpRequest1(link, pesquisa, "pais.html");
        
        // Leitura do arquivo local
        Scanner ler = new Scanner(new FileInputStream("pais.html"));
        String linha;
        
        // Expressões regulares para encontrar o nome do país
        String er1 = "<span class=\"mw-page-title-main\">([a-zA-Z\\s]+)<\\/span>"; // marcador de início
        Pattern p1 = Pattern.compile(er1);
        Matcher m1;
        
        while (ler.hasNextLine()) {
            linha = ler.nextLine();
            m1 = p1.matcher(linha);
            if (m1.find()) {
                ler.close();
                return m1.group(1);
            }
        }
        ler.close();
        return null;
    }
    
    public static String obtem_iso_pais(String pesquisa) throws FileNotFoundException, IOException {
        String link = "https://en.wikipedia.org/wiki/";

        HttpRequestFunctions.httpRequest1(link, pesquisa, "pais.html");

        Scanner ler = new Scanner(new FileInputStream("pais.html"));
        String linha;
        String er = "<td class=\"infobox-data\"><a[^>]*>([A-Z]+)<\\/a><\\/td>";
        Pattern p = Pattern.compile(er);
        Matcher m;

        while (ler.hasNextLine()) {
            linha = ler.nextLine();
            m = p.matcher(linha);
            if (m.find()) {
                ler.close();
                return m.group(1);
            }
        }
        ler.close();
        return null;
    }
    
    /*public static String obtem_iso_pais2(String pesquisa) throws FileNotFoundException, IOException {
        String link = "https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes";

        HttpRequestFunctions.httpRequest1(link, "", "pais.html");

        Scanner ler = new Scanner(new FileInputStream("pais.html"));
        String linha;
        String erPais = "<td data-sort-value=\"[^\"]*\"><a href=\"/wiki/" + pesquisa + "\" title=\"" + pesquisa + "\">[^\"]*</a>";
        String erISO = "<td><a href=\"/wiki/ISO_3166-1_alpha-2#([A-Z]{2})";
        
        Pattern pPais = Pattern.compile(erPais);
        Pattern pISO = Pattern.compile(erISO);
        Matcher mPais, mISO;

        while (ler.hasNextLine()) {
            linha = ler.nextLine();
            mPais = pPais.matcher(linha);
            if (mPais.find()) {
                while (ler.hasNextLine()) {
                    linha = ler.nextLine();
                    mISO = pISO.matcher(linha);
                    if (mISO.find()) {
                        ler.close();
                        return mISO.group(1);
                    }
                }
            }
        }
        ler.close();
        return null;
    }*/
     
    public static String obtem_continente_pais(String pesquisa) throws FileNotFoundException, IOException {
        String link = "https://web.archive.org/web/20240407193855/https://www.countryreports.org/country/" + pesquisa + ".htm";
        
        HttpRequestFunctions.httpRequest1(link, "", "pais.html");
        Scanner ler = new Scanner(new FileInputStream("pais.html"));
        String linha;
        String er1 = "\\s*Location\\s*</td>";
        // Expressão regular apenas com os nomes dos continentes
        String er2 = "\\s*(Africa|Asia|Central America|Europe|Middle East|North America|Oceania|South America|The Caribbean).*?</td>";
        Pattern p1 = Pattern.compile(er1);
        Pattern p2 = Pattern.compile(er2);
        Matcher m1, m2;

        while (ler.hasNextLine()) {
            linha = ler.nextLine();
            m1 = p1.matcher(linha);
            if (m1.find()) {
                linha = ler.nextLine(); // <td> lixo
                linha = ler.nextLine(); // procurar ER2
                m2 = p2.matcher(linha);
                if (m2.find()) {
                    String strAuxiliar = m2.group(1).trim();
                    if (strAuxiliar.equals("Middle East")) {
                        strAuxiliar = "Asia";
                    }
                    if (strAuxiliar.equals("The Caribbean") || strAuxiliar.equals("Caribbean")) {
                        strAuxiliar = "America";
                    }
                    if (strAuxiliar.equals("Central America")) {
                        strAuxiliar = "America";
                    }
                    if (strAuxiliar.equals("North America")) {
                        strAuxiliar = "America";
                    }
                    if (strAuxiliar.equals("South America")) {
                        strAuxiliar = "America";
                    }
                    ler.close();
                    return strAuxiliar;
                }
            }
        }
        ler.close();
        return null;
    }
       
    static String obtem_nomeGovernante_pais(String pesquisa) throws FileNotFoundException, IOException {
    String link = "https://en.wikipedia.org/wiki/";

    HttpRequestFunctions.httpRequest1(link, pesquisa, "pais.html");

    Scanner ler = new Scanner(new FileInputStream("pais.html"));
    String linha;
    String er = "<a href=\"/wiki/[^>]*\" title=\"[^\"]+\">(?:President|Monarch)<\\/a>.*?<td class=\"infobox-data\">(?:<span class=\"nowrap\">)?<a href=\"[^\"]+\" title=\"[^\"]+\">([^<]+)<\\/a>(?:<\\/span>)?<\\/td>";
    Pattern p = Pattern.compile(er);
    Matcher m;
    
    while (ler.hasNextLine()) {
        linha = ler.nextLine();
        m = p.matcher(linha);
        if (m.find()) {
            ler.close();
            return m.group(1);
        }
    }
    ler.close();
    return null;
    }
    
    static String obtem_linkBandeira_pais(String pesquisa) throws FileNotFoundException, IOException {
        
        String link = "https://en.wikipedia.org/wiki/";
        
        HttpRequestFunctions.httpRequest1(link, pesquisa, "pais.html");
        Scanner ler = new Scanner(new FileInputStream("pais.html"));
        String linha;
        String er1 = "image\" content=\"([^\"]+)\">"; // 
        Pattern p1 = Pattern.compile(er1);
         Matcher m1;
        while (ler.hasNextLine()) {
            linha = ler.nextLine();
            m1 = p1.matcher(linha);
            if (m1.find()) {
                ler.close();
                return m1.group(1);
            }
        }
        ler.close();
        return null;
    }
    
    static String obtem_capital_pais(String pesquisa) throws FileNotFoundException, IOException {
        
        String link = "https://en.wikipedia.org/wiki/";

        HttpRequestFunctions.httpRequest1(link, pesquisa, "pais.html");
        Scanner ler = new Scanner(new FileInputStream("pais.html"));
        String linha;
        String er1 = "class=\"infobox-label\">Capital.*?</th><td class=\"infobox-data\"><a href=\"[^\"]+\" title=\"[^\"]+\">([^<]+)</a>";
        Pattern p1 = Pattern.compile(er1);
        Matcher m1;
        while (ler.hasNextLine()) {
            linha = ler.nextLine();
            m1 = p1.matcher(linha);
            if (m1.find()) {
                ler.close();
                return m1.group(1);
            }
        }
        ler.close();
        return null;
    }
    
    static String obtem_moeda_pais(String pesquisa) throws FileNotFoundException, IOException {
        
        String link = "https://en.wikipedia.org/wiki/";

        HttpRequestFunctions.httpRequest1(link, pesquisa, "pais.html");
        Scanner ler = new Scanner(new FileInputStream("pais.html"));
        String linha;
        String er1 = "class=\"infobox-label\">Currency.*?</th><td class=\"infobox-data\"><a href=\"[^\"]+\" title=\"[^\"]+\">([^<]+)</a>";
        Pattern p1 = Pattern.compile(er1);
        Matcher m1;
        while (ler.hasNextLine()) {
            linha = ler.nextLine();
            m1 = p1.matcher(linha);
            if (m1.find()) {
                ler.close();
                return m1.group(1);
            }
        }
        ler.close();
        return null;
    }
    
    static String obtem_dominioInternet_pais(String pesquisa) throws FileNotFoundException, IOException {
        
        String link = "https://en.wikipedia.org/wiki/";

        HttpRequestFunctions.httpRequest1(link, pesquisa, "pais.html");
        Scanner ler = new Scanner(new FileInputStream("pais.html"));
        String linha;
        String er1 = "class=\"infobox-label\"><a href=\"/wiki/Country_code_top-level_domain\" title=\"Country code top-level domain\">Internet TLD</a></th><td class=\"infobox-data\"><a href=\"/wiki/(\\..+)\" title=\"\\..+\">\\..+</a>";
        Pattern p1 = Pattern.compile(er1);
        Matcher m1;
        while (ler.hasNextLine()) {
            linha = ler.nextLine();
            m1 = p1.matcher(linha);
            if (m1.find()) {
                ler.close();
                return m1.group(1);
            }
        }
        ler.close();
        return null;
    }
    
    static String obtem_idiomasOficiais_pais(String pesquisa) throws FileNotFoundException, IOException {
        
        String link = "https://en.wikipedia.org/wiki/";

        HttpRequestFunctions.httpRequest1(link, pesquisa, "pais.html");
        Scanner ler = new Scanner(new FileInputStream("pais.html"));
        String linha;
        String er1 = "\"infobox-label\">Official.*?<\\/th><td class=\"infobox-data\"><a href=\"\\/wiki\\/(.*?)\" title=\".*?\">(.*?)<\\/a>";
        
        Pattern p1 = Pattern.compile(er1);
        Matcher m1;
        
        while (ler.hasNextLine()) {
            linha = ler.nextLine();
            m1 = p1.matcher(linha);
            if (m1.find()) {
                ler.close();
                return m1.group(2);
            }
        }
        
        ler.close();
        return null;
    }
        
    static /*String*/ int obtem_populacao_pais(String pesquisa) throws FileNotFoundException, IOException {
        String link = "https://web.archive.org/web/20240407193855/https://www.countryreports.org/country/" + pesquisa + ".htm";
        
        HttpRequestFunctions.httpRequest1(link, "", "pais.html");
        Scanner ler = new Scanner(new FileInputStream("pais.html"));
        String linha;
        String er1 = "\\s*Population\\s*</td>";
        String er2 = "\\s*(.*?)\\s*</td>";
        Pattern p1 = Pattern.compile(er1);
        Pattern p2 = Pattern.compile(er2);
        Matcher m1, m2;
        while (ler.hasNextLine()) {
            linha = ler.nextLine();
            m1 = p1.matcher(linha);
            if (m1.find()) {
                linha = ler.nextLine(); // <td> lixo
                linha = ler.nextLine(); // procurar ER2
                m2 = p2.matcher(linha);
                if (m2.find()) {
                    ler.close();
                    /*return m2.group(1);*/
                    return Integer.parseInt(m2.group(1).replace(",", "").trim()); // Converter para int
                }
            }
        }
        ler.close();
        //return null;
        return -1;
    }
    
    static String obtem_areaKM2_pais(String pesquisa) throws FileNotFoundException, IOException {
        String link = "https://web.archive.org/web/20240407193855/https://www.countryreports.org/country/" + pesquisa + ".htm";
        
        HttpRequestFunctions.httpRequest1(link, "", "pais.html");
        Scanner ler = new Scanner(new FileInputStream("pais.html"));
        String linha;
        String er1 = "\\s*Total Area\\s*</td>";
        String er2 = "\\s*(\\d+(?:,\\d+)?)\\s+Square Kilometers\\s*</td>";
        Pattern p1 = Pattern.compile(er1);
        Pattern p2 = Pattern.compile(er2);
        Matcher m1, m2;
        while (ler.hasNextLine()) {
            linha = ler.nextLine();
            m1 = p1.matcher(linha);
            if (m1.find()) {
                linha = ler.nextLine(); // <td> lixo
                linha = ler.nextLine(); // procurar ER2
                m2 = p2.matcher(linha);
                if (m2.find()) {
                    ler.close();
                    return m2.group(1);
                }
            }
        }
        ler.close();
        return null;
    }
    
    static String obtem_crescimentoPopulacao_pais(String pesquisa) throws FileNotFoundException, IOException {
        String link = "https://web.archive.org/web/20240407193855/https://www.countryreports.org/country/" + pesquisa + ".htm";
        
        HttpRequestFunctions.httpRequest1(link, "", "pais.html");
        Scanner ler = new Scanner(new FileInputStream("pais.html"));
        String linha;
        String er1 = "\\s*Population Growth Rate\\s*</td>";
        String er2 = "\\s*(\\d+\\.\\d+)\\s*%?\\s*</td>";
        Pattern p1 = Pattern.compile(er1);
        Pattern p2 = Pattern.compile(er2);
        Matcher m1, m2;
        while (ler.hasNextLine()) {
            linha = ler.nextLine();
            m1 = p1.matcher(linha);
            if (m1.find()) {
                linha = ler.nextLine(); // <td> lixo
                linha = ler.nextLine(); // procurar ER2
                m2 = p2.matcher(linha);
                if (m2.find()) {
                    ler.close();
                    return m2.group(1);
                }
            }
        }
        ler.close();
        return null;
    }
    
    public static ArrayList<String> obtem_paisesVizinhos_pais(String pesquisa) throws FileNotFoundException, IOException {
        String link = "https://web.archive.org/web/20240407193855/https://www.countryreports.org/country/" + pesquisa + "/geography.htm";
        
        HttpRequestFunctions.httpRequest1(link, "", "pais.html");
        Scanner ler = new Scanner(new FileInputStream("pais.html"));
        ArrayList<String> lista = new ArrayList();
        String er1 = "\\s*Border Countries\\s*</td>";
        String er2 = "\\s*(.*?)\\s*</td>"; 
        Pattern p1 = Pattern.compile(er1);
        Pattern p2 = Pattern.compile(er2);
        Matcher m1, m2;
        while (ler.hasNextLine()) {
            String linha = ler.nextLine();
            m1 = p1.matcher(linha);
            if (m1.find()) {
                ler.nextLine();
                linha = ler.nextLine();
                m2 = p2.matcher(linha);
                while (m2.find()) {
                    lista.add(m2.group(1));
                }
            }
        }
        ler.close();
        
        if (lista.isEmpty()) {
        lista.add("Não tem vizinhos");
        }
        
        return lista;
    }
    
    public static ArrayList<String> obtem_cidadesPopulosas_pais(String pesquisa) throws FileNotFoundException, IOException {
        String link = "https://en.wikipedia.org/wiki/";
        
        HttpRequestFunctions.httpRequest1(link, pesquisa, "pais.html");
        Scanner ler = new Scanner(new FileInputStream("pais.html"));
        ArrayList<String> lista = new ArrayList();
        String er1 = "<td class=\"largestCities-cell-background\" style=\"text-align:center;\">([1-5])</td>";
        String er2 = "<td style=\"text-align:left;\"><a href=\"[^\"]*\" title=\"[^\"]*\">(.*?)</a></td>";
        Pattern p1 = Pattern.compile(er1);
        Pattern p2 = Pattern.compile(er2);
        Matcher m1, m2;
        while (ler.hasNextLine()) {
            String linha = ler.nextLine();
            m1 = p1.matcher(linha);
            if (m1.find()) {
                //ler.nextLine();
                linha = ler.nextLine();
                m2 = p2.matcher(linha);
                while (m2.find()) {
                    lista.add(m2.group(1));
                }
            }
        }
        ler.close();
        return lista;
    }

    
}