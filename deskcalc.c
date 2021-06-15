#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <cmath>

using namespace std;

enum Token_value {
  FUNCTION, NAME, NUMBER, END,
  PLUS = '+', MINUS = '-', MUL = '*', DIV = '/',
  PRINT = ';', ASSIGIN = '=', LP = '(', RP = ')',
  EXPO = '^', MLP = '[', MRP = ']', COMMA = ','
};

Token_value curr_tok = PRINT;

double number_value;
string string_value;
int no_of_errors;

map < string, double > table;


double error (const string & s);
double message (const string & s);
Token_value get_token ();
double prim (bool get);
double term (bool get);
double expr (bool get);


const char help[] = "DC: Desk Calculator\n\npress ctrl+Z to quit\n";

int
main (int argc, char *argv[])
{
  // Standard variables
  table["pi"] = 3.1415926535897932385;
  table["e"] = 2.7182818284590452354;
  // Default help message
  message (help);

  // Main loop
  while (cin) {
    get_token ();
    if (curr_tok == END)
      break;
    if (curr_tok == PRINT)
      continue;
    cout << expr(false) << '\n';
  }

  return no_of_errors;
}

double
message (const string & s)
{
  cout << s << '\n';
  return 0;
}

double
error (const string & s)
{
  no_of_errors++;
  cerr << "Error:" << s << '\n';
  return 1;
}

double
expr (bool get)
{
  double left = term (get);
  for (;;)
    switch (curr_tok) {
    case PLUS:
      {
	left += term (true);
	break;
      }
    case MINUS:
      {
	left -= term (true);
	break;
      }
    default:
      return left;
    }
}

double
term (bool get)
{
  double left = prim (get);
  for (;;)
    switch (curr_tok) {
    case MUL: {
      left *= term (true);
      break;
    }
    case DIV: {
      if (double d = prim (true)) {
	left /= d;
	break;
      }
      return error ("divide by 0");
    }
    case EXPO: {
      left = pow (left, prim (true));
      break;
    }
    default:
      return left;
    }
}

double
prim (bool get)
{
  if (get)
    get_token ();

  switch (curr_tok) {
  case MINUS: {
    return (-expr (true));
  }
  case NUMBER: {
    double v = number_value;
    get_token ();
    return v;
  }
  case NAME: {
    //help
    if (string_value == "help" || string_value == "Help"
	|| string_value == "HELP") {
      return message (help);
    }
    else if (string_value == "clear" || string_value == "Clear"
	     || string_value == "CLEAR") {
      cout.flush ();
      return message (help);
    }
    else
      //trignometry functions
      if (string_value == "sin") {
	if (get_token () == LP)
	  return sin (expr (true));
      }
      else if (string_value == "cos") {
	if (get_token () == LP)
	  return cos (expr (true));
      }
      else if (string_value == "tan") {
	if (get_token () == LP)
	  return tan (expr (true));
      }
      else if (string_value == "asin") {
	if (get_token () == LP)
	  return asin (expr (true));
      }
      else if (string_value == "acos") {
	if (get_token () == LP)
	  return acos (expr (true));
      }
      else if (string_value == "atan") {
	if (get_token () == LP)
	  return atan (expr (true));
      }
      // exponential functions
      else if (string_value == "exp") {
	if (get_token () == LP)
	  return exp (expr (true));
      }
      else if (string_value == "log") {
	if (get_token () == LP)
	  return log (expr (true));
      }
      else if (string_value == "log10") {
	if (get_token () == LP)
	  return log10 (expr (true));
      }
      else if (string_value == "sum") {
	get_token ();
	if (curr_tok == LP) {
	  double sum = 0;
	  sum += expr (true);
	  while (curr_tok != RP) {
	    if (curr_tok == COMMA)
	      sum += expr (true);
	  }
	  return sum;
	}
      }
    // new variables
      else {
	double &v = table[string_value];
	if (get_token () == ASSIGIN)
	  v = expr (true);
	return v;
      }
  }
  case LP: {
    double e = expr (true);
    if (curr_tok != RP)
      return error ("')' expected");
    get_token ();
    return e;
  }
  default:
    return error ("primary expected");
  }
}

Token_value
get_token ()
{
  char ch;

  do {
    if (!cin.get (ch))
      return curr_tok = END;
  } while (ch != '\n' && isspace (ch));

  switch (ch) {
  case '\n':
  case ';':
    return curr_tok = PRINT;
  case '^':
  case '*':
  case '/':
  case '+':
  case '-':
  case '(':
  case ')':
  case '[':
  case ']':
  case ',':
  case '=':
    return curr_tok = Token_value (ch);
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case '.':
    cin.putback (ch);
    cin >> number_value;
    return curr_tok = NUMBER;
  default:
    if (isalpha (ch)) {
      string_value = ch;
      while (cin.get (ch) && isalnum (ch))
	string_value.push_back (ch);
      cin.putback (ch);
      return curr_tok = NAME;
    }
    error ("bad token");
    return curr_tok = PRINT;
  }
}
