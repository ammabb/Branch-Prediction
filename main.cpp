#include <bits/stdc++.h>
#include <fstream>
#include <map>
#include <set>
#include <bitset>
#include <sstream>
#define int long long
using namespace std;
int PC = 0;
string hexToBinary(string hex)
{
    string binary = "";
    for (char &c : hex)
    {
        switch (c)
        {
        case '0':
            binary += "0000";
            break;
        case '1':
            binary += "0001";
            break;
        case '2':
            binary += "0010";
            break;
        case '3':
            binary += "0011";
            break;
        case '4':
            binary += "0100";
            break;
        case '5':
            binary += "0101";
            break;
        case '6':
            binary += "0110";
            break;
        case '7':
            binary += "0111";
            break;
        case '8':
            binary += "1000";
            break;
        case '9':
            binary += "1001";
            break;
        case 'A':
        case 'a':
            binary += "1010";
            break;
        case 'B':
        case 'b':
            binary += "1011";
            break;
        case 'C':
        case 'c':
            binary += "1100";
            break;
        case 'D':
        case 'd':
            binary += "1101";
            break;
        case 'E':
        case 'e':
            binary += "1110";
            break;
        case 'F':
        case 'f':
            binary += "1111";
            break;
        default:
            return "Invalid hexadecimal input";
        }
    }
    return binary;
}
int powers(int y)
{
   int p=1;
   for(int i=0;i<y;i++)
   {
    p*=2;
   }
   return p;
}
int binaryToDecimal(string binary)
{
    int decimal = 0;
    int power = 0;
    for (int i = binary.length() - 1; i >= 0; --i)
    {
        if (binary[i] == '1')
        {
            decimal += powers( power);
            // cout<<decimal<<"\n";
        }
        power++;
    }
    return decimal;
}

int subtractHex(string hex1, string hex2)
{
    int decimal1, decimal2;
    stringstream(hex1) >> hex >> decimal1;
    stringstream(hex2) >> hex >> decimal2;
    int resultDecimal = decimal1 - decimal2;
    stringstream ss;
    ss << hex << resultDecimal;
    string resultHex = ss.str();
    string resultBinary = hexToBinary(resultHex);
    return binaryToDecimal(resultBinary);
}
bool bOrNot(string opcode)
{
    if (opcode == "1100011")
        return true;
    else if (opcode == "1100111")
        return true;
    else if (opcode == "1101111")
        return true;
    else
        return false;
}
int instruction(string mc)
{
    string imm = "";
    imm = mc[0] + imm;
    imm = imm + mc[24];
    imm = imm + mc.substr(1, 6);
    imm = imm + mc.substr(20, 4);
    imm = imm + '0';
    return binaryToDecimal(imm);
}
int32_t main()
{
    ifstream file("trace.txt");
    string line;
    vector<string> v;
    // pc vector
    vector<string> pcv;
    // machine code vector
    vector<string> mcv;
    // No.of branch instruction
    int branch = 0;
    // Always taken case
    int taken = 0;
    // Always not taken case
    int not_taken = 0;

    while (getline(file, line))
    {
        v.push_back(line);
    }
    file.close();
    vector<char> vc;
    // Extracting PC and Machine code
    for (int i = 0; (i < v.size() && i < 1000000); i++)
    {
        // cout<<i<<"\n";
        for (int j = 0; j < v[i].size(); j++)
        {

            string pc = "";
            string mc = "";
            int k = 0;
            for (; k < v[i].size(); k++)
            {
                if (v[i][k] == ':')
                    break;
            }
            if (k == v[i].size())
                continue;
            k++;
            while (v[i][k] != 'x')
            {
                if (k == v[i].size())
                    break;
                k++;
            }
            if (v[i][k - 1] != '0' || k == v[i].size())
                continue;
            k++;
            for (int o = k; o < v[i].size(); o++)
            {
                if (v[i][o] == '(')
                {
                    o++;
                    k = o;
                    break;
                }
                if (v[i][o] != ' ')
                {
                    pc = pc + v[i][o];
                }
            }
            while (v[i][k] != 'x')
            {
                k++;
            }
            k++;
            for (int o = k; o < v[i].size(); o++)
            {

                if (v[i][o] == ')')
                {
                    break;
                }
                if (v[i][o] != ' ')
                {
                    mc = mc + v[i][o];
                }
            }
            pcv.push_back(pc);
            mcv.push_back(mc);
            break;
        }
    }
    // Calculating if brnch taken or not taken
    // Creating history table
    map<string, string> his_tab;
    // Creating Branch Target Buffer
    vector<pair<string, string>> BTB;
    for (int i = 0; i < mcv.size() - 1; i++)
    {
        int y = binaryToDecimal(hexToBinary(pcv[i]));
        int z = binaryToDecimal(hexToBinary(pcv[i + 1]));
        int diff = z - y;
        if (diff == 4)
        {
            string bin_inst = hexToBinary(mcv[i]);
            string opcode = "";
            for (int j = 25; j < 32; j++)
                opcode = opcode + bin_inst[j];
            bool h = bOrNot(opcode);
            if (h)
            {
                branch++;
                if (opcode == "1100111")
                {
                    // cout<<pcv[i]<<" "<<mcv[i]<<"\n";
                    taken++;
                    vc.push_back('T');
                    if (i != mcv.size() - 1)
                        BTB.push_back(make_pair(pcv[i], pcv[i + 1]));
                    else
                        BTB.push_back(make_pair(pcv[i], "-"));
                    his_tab[pcv[i]] = his_tab[pcv[i]] + 'T';
                }
                else if (opcode == "1101111")
                {
                    vc.push_back('T');
                    // cout<<pcv[i]<<" "<<mcv[i]<<"\n";
                    taken++;
                    if (i != mcv.size() - 1)
                        BTB.push_back(make_pair(pcv[i], pcv[i + 1]));
                    else
                        BTB.push_back(make_pair(pcv[i], "-"));
                    his_tab[pcv[i]] = his_tab[pcv[i]] + 'T';
                }
                else if (opcode == "1100011")
                {
                    // cout<<y<<"\n";
                    int imm = instruction(hexToBinary(mcv[i]));
                    if (imm == 0)
                    {
                        // cout<<pcv[i]<<" "<<mcv[i]<<"\n";
                        vc.push_back('T');
                        taken++;
                        if (i != mcv.size() - 1)
                            BTB.push_back(make_pair(pcv[i], pcv[i + 1]));
                        else
                            BTB.push_back(make_pair(pcv[i], "-"));
                        his_tab[pcv[i]] = his_tab[pcv[i]] + 'T';
                    }
                    else
                    {
                        vc.push_back('N');
                        not_taken++;
                        if (i != mcv.size() - 1)
                            BTB.push_back(make_pair(pcv[i], pcv[i + 1]));
                        else
                            BTB.push_back(make_pair(pcv[i], "-"));
                        his_tab[pcv[i]] = his_tab[pcv[i]] + 'N';
                    }
                }
            }
        }
        else
        {
            // cout<<pcv[i]<<" "<<mcv[i]<<"\n";
            vc.push_back('T');
            his_tab[pcv[i]] = his_tab[pcv[i]] + 'T';
            if (i != mcv.size() - 1)
                BTB.push_back(make_pair(pcv[i], pcv[i + 1]));
            else
                BTB.push_back(make_pair(pcv[i], "-"));
            taken++;
            branch++;
        }
    }
    //cout << taken << " " << branch << "\n";
    // Accuracy of always taken
    float acc = (float)taken / (float)branch;
    // Accuracy of Always not-taken
    float acc2 = (float)not_taken / (float)branch;
    // Accuracy for both taken and not_taken
    cout << "Overall accuracy of ALWAYS_TAKEN branch predictor is: " << acc * 100 << "\n";
    cout << "Overall accuracy of ALWAYS_NOT_TAKEN branch predictor is:" << acc2 * 100 << "\n";
    // cout<<acc<<" "<<acc2<<"\n";

    //
    // STARTING 1 BIT BRANCH PREDICTOR
    //
    int t = 0; // NOT_TAKEN and t==1 for TAKEN
    int hit = 0;
    int miss = 0;
    // Calculating hit and miss starting with not taken
    ////
    for (int i = 0; i < mcv.size() - 1; i++)
    {
        int y = binaryToDecimal(hexToBinary(pcv[i]));
        int z = binaryToDecimal(hexToBinary(pcv[i + 1]));
        int diff = z - y;
        if (diff == 4)
        {
            string bin_inst = hexToBinary(mcv[i]);
            string opcode = "";
            for (int j = 25; j < 32; j++)
                opcode = opcode + bin_inst[j];
            bool h = bOrNot(opcode);
            if (h)
            {
                branch++;
                if (opcode == "1100111")
                {
                    if (t == 1)
                    {
                        hit++;
                    }
                    else
                    {
                        miss++;
                        t = 1;
                    }
                }
                else if (opcode == "1101111")
                {
                    if (t == 1)
                    {
                        hit++;
                    }
                    else
                    {
                        miss++;
                        t = 1;
                    }
                }
                else if (opcode == "1100011")
                {
                    // cout<<y<<"\n";
                    int imm = instruction(hexToBinary(mcv[i]));
                    if (imm == 0)
                    {
                        if (t == 1)
                        {
                            hit++;
                        }
                        else
                        {
                            miss++;
                            t = 1;
                        }
                    }
                    else
                    {
                        if (t == 0)
                        {
                            hit++;
                        }
                        else
                        {
                            t = 0;
                            miss++;
                        }
                    }
                }
            }
        }
        else
        {
            if (t == 1)
            {
                hit++;
            }
            else
            {
                t = 1;
                miss++;
            }
        }
    }
    cout << "1-BIT BRANCH PREDICTOR TAKING FIRST PRDEICTION AS NOT TAKEN: ";
    cout << (float)(hit * 100) / (float)(hit + miss) << "\n";
    //cout << hit << " " << miss << "p\n";
    // 2-BIT BRANCH PREDICTOR
    //
    int bit_2 = 10;
    int count = 0;
    int hit2 = 0;
    int miss2 = 0;
    for (int i = 0; i < mcv.size() - 1; i++)
    {
        int y = binaryToDecimal(hexToBinary(pcv[i]));
        int z = binaryToDecimal(hexToBinary(pcv[i + 1]));
        int diff = z - y;
        if (diff == 4)
        {
            string bin_inst = hexToBinary(mcv[i]);
            string opcode = "";
            for (int j = 25; j < 32; j++)
                opcode = opcode + bin_inst[j];
            bool h = bOrNot(opcode);
            if (h)
            {
                // branch++;
                if (opcode == "1100111")
                {
                    if (bit_2 == 10)
                    {
                        bit_2 = 11;
                        hit2++;
                    }
                    else if (bit_2 == 1)
                    {
                        miss2++;
                        bit_2 = 10;
                    }
                    else if (bit_2 == 0)
                    {
                        miss2++;
                        bit_2 = 1;
                    }
                    else if (bit_2 == 11)
                    {
                        hit2++;
                    }
                }
                else if (opcode == "1101111")
                {
                    if (bit_2 == 10)
                    {
                        bit_2 = 11;
                        hit2++;
                    }
                    else if (bit_2 == 1)
                    {
                        miss2++;
                        bit_2 = 10;
                    }
                    else if (bit_2 == 0)
                    {
                        miss2++;
                        bit_2 = 1;
                    }
                    else if (bit_2 == 11)
                    {
                        hit2++;
                    }
                }
                else if (opcode == "1100011")
                {
                    // cout<<y<<"\n";
                    int imm = instruction(hexToBinary(mcv[i]));
                    if (imm == 0)
                    {
                        if (bit_2 == 10)
                        {
                            bit_2 = 11;
                            hit2++;
                        }
                        else if (bit_2 == 1)
                        {
                            miss2++;
                            bit_2 = 10;
                        }
                        else if (bit_2 == 0)
                        {
                            miss2++;
                            bit_2 = 1;
                        }
                        else if (bit_2 == 11)
                        {
                            hit2++;
                        }
                    }
                    else
                    {
                        if (bit_2 == 10)
                        {
                            bit_2 = 1;
                            miss2++;
                        }
                        else if (bit_2 == 1)
                        {
                            hit2++;
                            bit_2 = 0;
                        }
                        else if (bit_2 == 0)
                        {
                            hit2++;
                        }
                        else if (bit_2 == 11)
                        {
                            bit_2 = 10;
                            miss2++;
                        }
                    }
                }
                // cout << hit2 << " " << miss2 << "\n";
            }
        }
        else
        {
            if (bit_2 == 10)
            {
                bit_2 = 11;
                hit2++;
            }
            else if (bit_2 == 1)
            {
                miss2++;
                bit_2 = 10;
            }
            else if (bit_2 == 0)
            {
                miss2++;
                bit_2 = 1;
            }
            else if (bit_2 == 11)
            {
                hit2++;
            }
            // cout << hit2 << " " << miss2 << "\n";
        }
    }
    cout << "2-BIT BRANCH PREDICTOR TAKING FIRST PRDEICTION AS WEAKLY TAKEN: ";
    cout << (float)(hit2 * 100) / (float)(hit2 + miss2) << "\n";
    //cout << hit2 << " " << miss2 << "\n";
    // PRINTING HISTORY TABLE
    ofstream ut("history.txt");
    ut << "HISTORY TABLE\n";
    ut << "BRANCH PC        HISTORY\n";
    for (auto it : his_tab)
    {
        ut << "0x" << it.first << "        " << it.second << "\n";
    }
    ofstream out("btb.txt");
    out << "BRANCH TARGET BUFFER\n";
    out << "BRANCH PC        TARGET PC\n";
    for (auto it : BTB)
    {
        out << "0x" << it.first << "       " << it.second << "\n";
    }
    ofstream ou("branch.txt");
    ou << "BRANCH OUTCOME\n";
    for (int i = 0; i < vc.size(); i++)
        ou << vc[i] << "\n ";
    ou << "\n";
}