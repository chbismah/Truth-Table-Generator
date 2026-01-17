// Name: Bismah Chaudhry
// Roll no: 25i-2079
// SE_C
#include <iostream>
#include <fstream>
using namespace std;

int tokenize(char input[], char tokens[]);
int precedence(char op);
int infix_to_postfix(char tokens[], int token_count, char output[]);

int tokenize(char input[], char tokens[])
{
    int count = 0;
    int openb = 0, closeb = 0;

    for (int i = 0; input[i] != '\0'; i++)
    {
        char ch = input[i];
        // skips the spaces
        if (ch == ' ')
        {
            continue;
        }
        if (ch >= 'A' && ch <= 'Z')
        {
            tokens[count++] = ch;
        }
        else if (ch == '(')
        {
            tokens[count++] = ch;
            openb++;
        }
        else if (ch == ')')
        {
            tokens[count++] = ch;
            closeb++;

            if (closeb > openb)
            {
                return -1; // too many bracktes so we return it because it is invalid
            }
        }
        else if (ch == '!' || ch == '&' || ch == '|' || ch == '>' || ch == '=')
        {
            tokens[count++] = ch;
        }
        else
        {
            // the character is invalid
            return -1;
        }
    }
    if (openb != closeb)
    {
        // check the brackets if they are colsed or are extra
        return -1;
    }
    return count;
}

// Operator precedence
int precedence(char op)
{
    if (op == '!')
        return 5;
    if (op == '&')
        return 4;
    if (op == '|')
        return 3;
    if (op == '>')
        return 2;
    if (op == '=')
        return 1;
    return 0;
}

// Infix to Postfix
int infix_to_postfix(char tokens[], int token_count, char output[])
{
    char stack[100];
    int top = -1;
    int p = 0; // kepping this for the index of output

    for (int i = 0; i < token_count; i++)
    {
        char c = tokens[i];
        if (c >= 'A' && c <= 'Z')
        {
            output[p++] = c;
        }
        else if (c == '(')
        {
            stack[++top] = c;
        }
        // Pop operators from the stack and append to output until a ( is found
        else if (c == ')')
        {
            // Pop until ( is found
            while (top >= 0 && stack[top] != '(')
            {
                output[p++] = stack[top--];
            }

            // No matching (
            if (top < 0)
                return -1;
            top--; // discard (
        }

        // Operator
        else
        {
            // pop the oerator the top of the stack unitl it has higher precedence
            while (top >= 0 && precedence(stack[top]) >= precedence(c) && stack[top] != '(')
            {
                output[p++] = stack[top--];
            }
            // push the operator to stack
            stack[++top] = c;
        }
    }
    // Pop remaining operators
    while (top >= 0)
    {
        if (stack[top] == '(')
        {
            return -3;
        }
        output[p++] = stack[top--];
    }
    return p;
}
bool apply_operator(char op, bool a, bool b)
{
    if (op == '&')
        return a && b;
    if (op == '|')
        return a || b;
    if (op == '>')
        return (!a) || b;
    if (op == '=')
        return a == b;
    return false;
}

bool evaluate_postfix(char postfix[], int postfix_len,
                      int var_count, char vars[], int values[])
{
    bool stack[50];
    int top = -1;

    for (int i = 0; i < postfix_len; i++)
    {
        char c = postfix[i];
        if (c >= 'A' && c <= 'Z')
        {
            bool val = false;

            for (int j = 0; j < var_count; j++)
            {
                if (vars[j] == c)
                {
                    val = (values[j] == 1);
                    break;
                }
            }

            stack[++top] = val;
        }
        else
        {
            if (c == '!')
            {
                if (top < 0)
                    return false;

                bool a = stack[top--];
                stack[++top] = !a;
            }
            else
            {
                if (top < 1)
                    return false;

                bool b = stack[top--];
                bool a = stack[top--];

                bool result = apply_operator(c, a, b);

                stack[++top] = result;
            }
        }
    }
    if (top != 0)
        return false;

    return stack[top];
}
void generate_truth_table(char vars[], int var_count, char postfix[], int postfix_len)
{
    int values[5];
    int total_rows = 1;
    for (int i = 0; i < var_count; i++)
        total_rows *= 2;
    cout << "\nTruth Table:\n";
    for (int i = 0; i < var_count; i++)
    {
        cout << vars[i] << " ";
    }
    cout << "| Result\n";
    for (int row = 0; row < total_rows; row++)
    {
        int temp = row;
        for (int i = var_count - 1; i >= 0; i--)
        {
            values[i] = temp % 2;
            temp /= 2;
        }
        for (int i = 0; i < var_count; i++)
        {
            cout << values[i] << " ";
        }
        bool result = evaluate_postfix(postfix, postfix_len, var_count, vars, values);

        cout << "| " << result << "\n";
    }
}
void save_csv(char vars[], int var_count, char postfix[], int postfix_len, string filename, string exp)
{
    ofstream fout(filename);
    if (!fout)
    {
        cout << "Cannot open file!\n";
        return;
    }

    int values[5];
    int total_rows = 1;
    for (int i = 0; i < var_count; i++)
        total_rows *= 2;

    // Write headers
    fout << "ALRS Output\n";
    fout << "Expression:," << exp << "\n";

    fout << "Variables:,";
    for (int i = 0; i < var_count; i++)
        fout << vars[i] << (i == var_count - 1 ? "" : ",");
    fout << "\n---------------------------------\n";

    // Truth table header row
    for (int i = 0; i < var_count; i++)
        fout << vars[i] << ",";
    fout << "Result\n";

    // Table rows
    for (int row = 0; row < total_rows; row++)
    {
        int temp = row;
        for (int i = var_count - 1; i >= 0; i--)
        {
            values[i] = temp % 2;
            temp /= 2;
        }
        for (int i = 0; i < var_count; i++)
            fout << values[i] << ",";
        bool result = evaluate_postfix(postfix, postfix_len, var_count, vars, values);
        fout << result << "\n";
    }

    fout.close();
    cout << "Saved to " << filename << "\n";
}

void save_txt(char vars[], int var_count, char postfix[], int postfix_len, string filename, string exp)
{
    ofstream fout(filename);
    if (!fout)
    {
        cout << "Cannot open file!\n";
        return;
    }

    int values[5];
    int total_rows = 1;
    for (int i = 0; i < var_count; i++)
        total_rows *= 2;

    fout << "ALRS Output\n";
    fout << "Expression: " << exp << "\n";
    fout << "Variables: ";
    for (int i = 0; i < var_count; i++)
        fout << vars[i] << " ";
    fout << "\n---------------------------------\n";

    // Header row
    for (int i = 0; i < var_count; i++)
        fout << vars[i] << " ";
    fout << "| Result\n";

    // Truth table rows
    for (int row = 0; row < total_rows; row++)
    {
        int temp = row;
        for (int i = var_count - 1; i >= 0; i--)
        {
            values[i] = temp % 2;
            temp /= 2;
        }
        for (int i = 0; i < var_count; i++)
            fout << values[i] << " ";
        bool result = evaluate_postfix(postfix, postfix_len, var_count, vars, values);
        fout << "| " << result << "\n";
    }

    fout.close();
    cout << "Saved to " << filename << "\n";
}
int main()
{
    char exp[200];
    char tokens[200];
    char postfix[200];

    int menu = 0; // FIXED: initialize menu

    while (menu != 6)
    {
        cout << "====================== Truth Table Generator =====================\n";
        cout << " OPERATORS:\n";
        cout << "  ! : NOT         & : AND         | : OR\n";
        cout << "  > : IMPLIES     = : EQUIVALENT  ( ) : Parentheses\n";
        cout << "------------------------------------------------------------------\n";
        cout << "\n--- MAIN MENU ---\n";
        cout << "1. Generate truth table for expression\n";
        cout << "2. View/ Load saved results\n";
        cout << "2. Exit\n";
        cout << "Choose: ";
        cin >> menu;

        switch (menu)
        {
        case 1:
        {
            cout << "\n--- Generate Truth Table ---\n";
            cout << "Enter logical Expression: ";
            cin.ignore();
            cin.getline(exp, 200);

            int tok_count = tokenize(exp, tokens);
            if (tok_count < 0)
            {
                cout << "Invalid expression!\n";
                break;
            }

            int post_len = infix_to_postfix(tokens, tok_count, postfix);
            if (post_len < 0)
            {
                cout << "Postfix conversion failed!\n";
                break;
            }

            // find unique variables
            char vars[10];
            int var_count = 0;

            for (int i = 0; i < tok_count; i++)
            {
                if (tokens[i] >= 'A' && tokens[i] <= 'Z')
                {
                    bool exists = false;
                    for (int j = 0; j < var_count; j++)
                    {
                        if (vars[j] == tokens[i])
                        {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists)
                        vars[var_count++] = tokens[i];
                }
            }

            generate_truth_table(vars, var_count, postfix, post_len);
            int save;
            char select;
            cout << "Do you want to save the file(Y/N): " << endl;
            cin>>select;
            if (select=='Y'|| select=='y')
            {
                cout << "\nSave truth table?\n1. TXT\n2. CSV\nChoose: ";
                cin >> save;
               if (save == 1)
                {
                    save_txt(vars, var_count, postfix, post_len, "truth_table.txt", exp);
                }
                else if (save == 2)
                {
                    save_csv(vars, var_count, postfix, post_len, "truth_table.csv",exp);
                }
            }
            else{
                break;
            }
            
            break;
        }

        case 2:
        {
            cout << "\n--- View / Load Saved Results ---\n";
            char filename[100];
            cout << "Enter the filename to view (e.g., result.txt): ";
            cin.ignore();
            cin.getline(filename, 100);

            ifstream fin(filename);
            if (!fin)
            {
                cout << "Cannot open file: " << filename << "\n";
                break;
            }

            string line;
            cout << "\nContents of " << filename << ":\n\n";
            while (getline(fin, line))
            {
                cout << line << "\n";
            }

            fin.close();
            break;
        }
        case 3:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid option!\n";
        }
    }

    return 0;
}
