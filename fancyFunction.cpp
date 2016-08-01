#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "fancyFunction.h"

#include <fstream>
#include <iomanip>
#include <string>
#include <map>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

// your task is to modify this class as needed
// please keep in mind to write a full documentation
/*! \brief short comment
 *
 *  detailed description
 */
template<class T>
class Database
{
    private:
        fs::path db_root = "./.db/";  // root folder for database tree
        std::string dumpfilename = "dump.db";  // dump filename
        int precision = 10;  // decimal precision for path and data variables

        /*! \brief short comment
         *
         *  detailed description
         */
        std::string stringify(T const & val)
        {
            std::stringstream ss;
            ss << std::setprecision(precision) << std::fixed << val;
            return ss.str();
        }

        /*! \brief short comment
         *
         *  detailed description
         */
        fs::path vectorToPath(std::vector<T> const & vec)
        {
            // transform vector to path:
            fs::path index_path = db_root;
            for(size_t i = 0; i != vec.size(); i++) {
                //std::cout << v.at(i);  // debug output
                if (i == vec.size() - 1) {  // last element (leaf node)
                    //std::cout << ", leaf element." << std::endl;  // debug output
                    index_path.append(stringify(vec.at(i)) + ".leaf");
                }
                else {  // all intermediate elements (path nodes)
                    //std::cout << ", path element." << std::endl;  // debug output
                    index_path.append(stringify(vec.at(i)));
                }
            }
            //std::cout << "search path: " << index_path << std::endl;  // debug output
            return index_path;
        }

    public:
        /*! \brief short comment
         *
         *  detailed description
         */
        T retrieve( std::vector<T> const & vec)
        {
            // transform vector to path:
            fs::path search_path = vectorToPath(vec);

            // check if path exists and retrieve data:
            bool existing = fs::exists(search_path);
            if (existing) {
                //std::cout << "existing" << std::endl;  // debug output
                double value;
                // open and read file
                std::ifstream infile;
                infile.open(search_path);
                if (infile.is_open()) {
                    // read first line and convert to double
                    std::string first_line;
                    getline(infile, first_line);
                    //std::cout << "first_line: " << first_line << std::endl;  // debug output
                    value = stod(first_line);
                }
                else {
                    throw std::ios_base::failure("unable to open file!");
                }
                infile.close();

                // return data read from file:
                return value;
            }
            else {
                throw std::out_of_range( "value not found in db!" );
            }
        }

        /*! \brief short comment
         *
         *  detailed description
         */
        void add( std::vector<T> const & vec, T const & val)
        {
            // transform vector to path:
            fs::path search_path = vectorToPath(vec);

            // find out if path already exists:
            bool existing = fs::exists(search_path);
            if (existing) {
                throw std::logic_error("path already exists!");
            }
            else {
                // create folder structure and write data file:
                //std::cout << "parent_path: " << search_path.parent_path() << std::endl;  // debug output
                //std::cout << "search_path: " << search_path << std::endl;  // debug output
                fs::create_directories(search_path.parent_path());
                std::ofstream outfile;
                outfile.open(search_path);
                if (outfile.is_open()) {
                    //std::cout << "writing data..." << std::endl;  // debug output
                    outfile << std::setprecision(precision) << std::fixed
                            << val << std::endl;
                } else {
                    throw std::ios_base::failure("unable to open file!");
                }
                outfile.close();
            }
        }

        /*! \brief short comment
         *
         *  detailed description
         */
        void dump()
        {
            std::map<std::string, std::string> dbmap;
            // create map from folder structure
            for(auto& entry: fs::recursive_directory_iterator(db_root)) {
                fs::path current_path = entry.path();
                //std::cout << "current path: " << current_path << std::endl;  // debug output
                if (current_path.extension() == ".leaf") {
                    //std::cout << ", leaf file." << std::endl;  // debug output
                    std::string key = current_path.parent_path();
                    key += "/";
                    key += current_path.stem();
                    std::string root_string = db_root;
                    key = key.substr(root_string.size());
                    //std::cout << "key: " << key << std::endl;  // debug output

                    std::string value;
                    std::ifstream infile;
                    infile.open(current_path);
                    if (infile.is_open()) {
                        // read first line and convert to double
                        getline(infile, value);
                    }
                    else {
                        throw std::ios_base::failure("unable to open file!");
                    }
                    infile.close();
                    //std::cout << "val: " << value << std::endl;  // debug output

                    // add to dumpfile:
                    dbmap.insert(make_pair(key, value));
                }
            }

            // write map to file:
            std::ofstream outfile;
            outfile.open(dumpfilename);
            if (outfile.is_open()) {
                for (auto it : dbmap) {
                    outfile << it.first << "=" << it.second << std::endl;
                }
            }
            else {
                throw std::ios_base::failure("unable to open file!");
            }
            outfile.close();
        }
};

/*! \brief short comment
 *
 *  detailed description
 */
int main( int argc, char ** argv )
{
    // transform strings passed to executable into vector<type>
    // any un-transformable type becomes 1.0
    auto values = transformStringsToValues<double>( argc, argv );

    // setup your database:
    Database<double> db;

    double result = 0;

    // check if arguments already in database
    // if yes return result from database
    // else calculate result
    try
    {
        result = db.retrieve( values );
    }
    catch( const std::out_of_range & err )
    {
        std::cerr << err.what() << std::endl;
        result = superFancyFunction( values );

        //save result to database
        db.add( values, result );
    }

    std::cout << std::setprecision(10) << std::fixed << result << std::endl;

    db.dump();

    return 0;
}
