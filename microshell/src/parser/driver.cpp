#include "driver.hh"
#include "parser.hh"

// For listing directory
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

//////
// Parsing driver
//////

Driver::Driver (DefaultDict* variables)
  : trace_scanning (false), trace_parsing (false) {
    this->variables = variables;
  }

int Driver::parse (const std::string& target) {

  this->target = target + "\n";
  location.initialize (&(this->target));
  scan_begin ();
  yy::parser parse (*this);
  parse.set_debug_level (trace_parsing);
  int res = parse ();
  scan_end ();
  return res;

}

std::string Driver::insert_variable(const std::string &line) {
  return variables->at(line.substr(1));
}

// Yes, I have written custom parsers - because I am too dump to
// realise how to add it into grammar.

////
// Variables
////
std::string Driver::insert_multi_variables(const std::string &line) {

  std::string result, var;

  size_t last_el = 0, first_space = 0;

  for (size_t i = line.find("$", last_el); i != std::string::npos;
       i = line.find("$", last_el)) {

    result += line.substr( last_el, i-last_el );

    first_space = line.find(" ", i);

    if (first_space != std::string::npos)
      var = insert_variable( line.substr(i, first_space-i) );
    else {
      var = insert_variable( line.substr(i) );
      result += var;
      last_el = line.size();
      break;
    }

    result += var;
    last_el = first_space+1;
  }

  result += line.substr(last_el);

  return result;
}

////
// Now for regular expressions
////

int getdir (std::string dir, std::vector<std::string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        return errno;
    }

    std::string temp;
    while ((dirp = readdir(dp)) != NULL) {
        temp = std::string(dirp->d_name);
        if (temp != ".." && temp != ".")
          files.push_back(temp);
    }
    closedir(dp);
    return 0;
}

bool isdir( const std::string& path ) {

    DIR *pDir;
    bool bExists = false;

    pDir = opendir (path.c_str());

    if (pDir != NULL)
    {
        bExists = true;
        (void) closedir (pDir);
    }

    return bExists;
}

int Driver::parse_part(size_t curr_token, std::string path,
  std::vector<std::string>& tokens, std::vector<std::string>& answer) {

  assert( curr_token < tokens.size() );

  // Listing our directory
  std::vector<std::string> dir_files;
  getdir(path, dir_files);

  //std::for_each(dir_files.begin(), dir_files.end(), [](std::string tok) {std::cout << tok << " ";});
  // Parsing regular expression token [^|<>\"=$ \t\n]

  std::string final_token = tokens[curr_token];

  std::string::size_type n = 0;

  std::string all_reg("[^|<>\"=$ \t\n]*");
  std::string one_reg("[^|<>\"=$ \t\n]");

  while ( (n = final_token.find("*", n)) != std::string::npos ) {
    final_token.replace(n, 1, all_reg);
    n += all_reg.size();
  }

  n = 0;

  while ( (n = final_token.find("?", n)) != std::string::npos ) {
    final_token.replace(n, 1, one_reg);
    n += one_reg.size();
  }

  std::regex path_regex(final_token);

  std::vector<std::string> results;

  for (auto file: dir_files) {
    if (std::regex_match(file, path_regex)){
      results.push_back( path + file );
    }
  }

  // Now there are two ways
  if ( curr_token == tokens.size() - 1 ) {
    //exiting recursion
    answer.insert( answer.end(), results.begin(), results.end() );
    return 0;
  }

  //going to the next level of folders
  for ( auto res: results ) {
    if ( isdir(res) )
      parse_part(curr_token + 1, res + '/', tokens, answer);
  }

  return 0;
}

std::vector<std::string> Driver::parse_reg(const std::string &line) {
  std::vector<std::string> tokens;
  std::vector<std::string> ans;

  // Lexer
  size_t pos = 0;
  std::string token;
  std::string s(line);

  while ((pos = s.find("/")) != std::string::npos) {
      tokens.push_back( s.substr(0, pos) );
      s.erase(0, pos + 1);
  }
  tokens.push_back(s);

  // Parser
  size_t curr_token = 0;
  std::string path("");

  if (tokens[0] == "") {
    curr_token = 1; path = "/";
  } else {
    path = "./";
  }

  parse_part(curr_token, path, tokens, ans);

  return ans;
}
