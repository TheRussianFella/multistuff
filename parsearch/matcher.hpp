#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <cassert>


////////////
//// Automata building
////////////

std::vector<int> prefix_function (const std::string& s) {
	int n = (int) s.length();
	std::vector<int> pi (n);
	for (int i=1; i<n; ++i) {
		int j = pi[i-1];
		while (j > 0 && s[i] != s[j])
			j = pi[j-1];
		if (s[i] == s[j])  ++j;
		pi[i] = j;
	}
	return pi;
}

std::vector<std::vector<int>> create_automata(const std::string& s) {

  assert( s.back() == '#' );

  const int alphabet = 256;

  int n = (int) s.length();
  std::vector<int> pi = prefix_function (s);
  std::vector < std::vector<int> > aut (n, std::vector<int> (alphabet));

  for (int i=0; i<n; ++i) {
  	for (unsigned char c=0; c<alphabet-1; ++c) {
  		if (i > 0 && c != s[i]) {
  			aut[i][c] = aut[pi[i-1]][c];
      } else {
  			aut[i][c] = i + (c == s[i]);
      }
    }
  }

  return aut;

}

////////////
//// Matcher
////////////


class FileMatcher {

private:

  const int alphabet = 256;
  std::vector < std::vector<int> > automata_table;


public:

  FileMatcher(const std::string& query) {
    automata_table = create_automata(query + "#");
  }

  void operator()(ParallelQueue<std::string>& shared_queue, ParallelQueue<std::pair<std::string, int>>& output_queue,
    std::shared_future<void> death_signal) {

      while ( death_signal.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout ){
        try {

          std::string file_path(shared_queue.pop());
          std::ifstream is( file_path );
          std::string temp; int line_num = 0; int state = 0;

          while ( std::getline(is, temp) && ++line_num ) {
            state = 0;
            for (unsigned char c: temp) {
              state = automata_table[state][c];
              if ( state == automata_table.size()-1 ) {
                output_queue.push( std::make_pair( file_path, line_num ));
                break;
              }
            }

          }

        } catch (const std::runtime_error& e){

          if (e.what() != std::string("timeout"))
            throw e;

        }
      }

      //std::cout << "Thread terminated...\n";
    }

};
