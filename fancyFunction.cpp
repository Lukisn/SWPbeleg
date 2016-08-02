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

/*! \file */

/*!
 * \mainpage Documentation Main Page
 *
 * \section Overview
 * This project is a short demo of a concept for a simple database like
 * application. This simple command line tool written in C++ handles lookups
 * and writing to a folder structure base database. Additionally it dumps the
 * current database state to a single file for further investigation.
 *
 * The projects central point is the Makefile that is used compile the source
 * code and clean the entire directory. Additionally it has two targets for
 * running a simple testing bash-script and creating the documentation with
 * Doxygen.
 *
 * \section Usage
 * According to the above mentioned structure of the project the following
 * usage is suggested:
 *
 * - at first compile the source by simply using makes default target.
 * "$> make"
 *
 * - if that succesfully runs the test-script can be run.
 * "$> make test"
 * This runs the test.sh script that calls the command line interface a couple
 * of times to create a small database.The dumped data is then processed further
 * with a python script to prepare it for plotting the contained 2d data with
 * gnuplot. As final step gnuplot is run to show the databases contents in 2d.
 *
 * - The last possible target in the Makfile is
 * "$> make docs"
 * This created the HTML documentation using Dpxygen in the doc/html/ folder.
 */

/*!
 * \class Database
 * \brief database class.
 *
 * This class handles the basic actions implemented to act as a simple
 * database. These actions are:
 * - retrieve: retrieve an element from the database
 * - add: add an element to the database
 * - dump: dump the complete database to a single ascii filesystem
 *
 * The database is implemented as a hidden folder structure with the root
 * folder beeing ".db/" (just like .git/ in git). The coordinates of the
 * database are arbitrarily long sequences of floating point numbers. These
 * coordinates make up the folders along the path to the actual data entry.
 * The entry is indicated by a file with the ending ".leaf". This file contains
 * the data that is stored in the database.
 *
 * example path: .db/1.000000/2.000000/5.000000/3.000000.leaf
 * example data in ".../3.000000.leaf": "30.000000"
 *
 * A dump of the entire database is created in a file called "dump.db".
 * In this fileeach entry is placed on a single line in the following
 * structure: "key=value". The key is the path of the data element and the
 * value is the actual data. For example (like above):
 *
 * 1.000000/2.000000/5.000000/3.000000=30.000000
 */
template<class T>
class Database
{
    private:
        fs::path db_root = "./.db/";  /*!< root folder name for database tree */
        std::string dumpfilename = "dump.db";  /*!< dump filename */
        int precision = 10;  /*!< decimal precision for path and data variables */

        /*!
         * \fn std::string stringify(T const & val)
         * \brief private helper function for consistend stringifying of a
         * floating point value.
         *
         * This function provides a consistend means to create strings from
         * floating point values. It does this by using a predefined fixed
         * precision defined within the class.
         *
         * \param val floating point value to be stringified.
         * \return string version of floating point value.
         */
        std::string stringify( T const & val )
        {
            std::stringstream ss;
            ss << std::setprecision( precision ) << std::fixed << val;
            return ss.str();
        }

        /*!
         * \fn fs::path vectorToPath( std::vector<T> const & vec )
         * \brief private helper funtion for converting a vector to a path.
         *
         * This function takes a vector and creates a path from its elements.
         * The path consists of the vectors elements as intermediate folders
         * and a *.leaf file for the last entry of the vector. This path acts
         * as the location for storing the data belonging to this index vector.
         *
         * \param vector of arbitrary length
         * \returns path object (fs::path) with vector elements as intermediate
         * folders
         */
        fs::path vectorToPath( std::vector<T> const & vec )
        {
            // transform vector to path:
            fs::path index_path = db_root;
            for(size_t i = 0; i != vec.size(); i++) {
                if (i == vec.size() - 1) {  // last element (leaf node)
                    index_path.append(stringify(vec.at(i)) + ".leaf");
                }
                else {  // all intermediate elements (path nodes)
                    index_path.append(stringify(vec.at(i)));
                }
            }
            //std::cout << "search path: " << index_path << std::endl;
            return index_path;
        }

    public:
        /*!
         * \fn T retrieve( std::vector<T> const & vec )
         * \brief Function for retrieving data elements from the database.
         *
         * This function tries to find the data entry for the given coordinate
         * vector. If the database contains a value, this value is returned.
         * Otherwise a std::out_of_range exception is thrown to indicate, that
         * no data can be retrieved for the given key vector.
         *
         * \param vec index vector to look up.
         * \returns value for index vector stored in the database.
         *
         * \throws std::out_of_range exception.
         * \throws std::ios_base::failure exception.
         */
        T retrieve( std::vector<T> const & vec )
        {
            // transform vector to path:
            fs::path search_path = vectorToPath( vec );

            // check if path exists and retrieve data:
            bool existing = fs::exists( search_path );
            if ( existing ) {
                double value;
                // open file and read first line:
                std::ifstream infile;
                infile.open( search_path );
                if ( infile.is_open() ) {
                    std::string first_line;
                    getline( infile, first_line );
                    value = stod( first_line );
                }
                else {
                    throw std::ios_base::failure( "unable to open file!" );
                }
                infile.close();
                // return data read from file:
                return value;
            }
            else {  // not existing:
                throw std::out_of_range( "value not found in db!" );
            }
        }

        /*!
         * \fn void add( std::vector<T> const & vec, T const & val )
         * \brief Function for adding data to the database.
         *
         * This Function tries to add new data to the database. If the key
         * vector already exists in the data base a logic_error exception is
         * thrown. Otherwise the value is pushed to the database by writing
         * the accoring path and file to the folder structure.
         *
         * \param vec index vector to store data for.
         * \param val data value to store in the database.
         *
         * \throws std::logic_error exception.
         * \throws std::ios_base::failure exception.
         */
        void add( std::vector<T> const & vec, T const & val )
        {
            // transform vector to path:
            fs::path search_path = vectorToPath( vec );

            // find out if path already exists:
            bool existing = fs::exists( search_path );
            if ( existing ) {
                throw std::logic_error("path already exists!");
            }
            else {  // not existing:
                // create folder structure:
                fs::create_directories( search_path.parent_path() );
                // open and write output data file:
                std::ofstream outfile;
                outfile.open( search_path );
                if (outfile.is_open()) {
                    outfile << std::setprecision( precision ) << std::fixed
                            << val << std::endl;
                } else {
                    throw std::ios_base::failure( "unable to open file!" );
                }
                outfile.close();
            }
        }

        /*!
         * \fn void dump()
         * \brief Function for dumping the database folder structure to a single file.
         *
         * This function traverses the entire database folder structure
         * recursively and dumps the data to a single ascii file. This file is
         * constructed by writing a key=value pair on each line. The key is the
         * former path in the database and the value is the content of the
         * *.leaf file.
         *
         * \throws std::ios_base::failure exception.
         */
        void dump()
        {
            std::map<std::string, std::string> dbmap;
            // create map from folder structure by traversing it recursively:
            for ( auto& entry: fs::recursive_directory_iterator( db_root ) ) {
                fs::path current_path = entry.path();
                if ( current_path.extension() == ".leaf" ) {
                    // create key from path:
                    std::string key = current_path.parent_path();
                    key += "/";
                    key += current_path.stem();
                    std::string root_string = db_root;
                    key = key.substr( root_string.size() );
                    // read value from file:
                    std::string value;
                    std::ifstream infile;
                    infile.open( current_path );
                    if (infile.is_open()) {
                        // read first line and convert to double
                        getline( infile, value );
                    }
                    else {
                        throw std::ios_base::failure( "unable to open file!" );
                    }
                    infile.close();
                    // add key/value pair to dumpfile:
                    dbmap.insert( make_pair( key, value ) );
                }
            }
            // write map to file:
            std::ofstream outfile;
            outfile.open( dumpfilename );
            if ( outfile.is_open() ) {
                for ( auto it : dbmap ) {
                    outfile << it.first << "=" << it.second << std::endl;
                }
            }
            else {
                throw std::ios_base::failure( "unable to open file!" );
            }
            outfile.close();
        }
};

/*!
 * \fn int main( int argc, char ** argv )
 * \brief Main function.
 *
 * This function contains the behaviour of the command line program created
 * using the Database class.
 *
 * The basic usage of this simple command line utility is the following:
 * $> fancy.x [c1, [c2, [c2, [...]]]]
 *
 * where ci's make the arbitrarily long coordinate vector of the data element
 * in question. If this element is already stored it is simply returned. If it
 * does not yet exist, a super fancy function called "superFancyFunction" is
 * called on the input vector. The result of that function is then stored in
 * the database folder structure for later retrieval. Every time the program
 * is run a dump of the current state of the database is created. so the folder
 * structure and the single file dump are always in sync.
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

    // output result:
    std::cout << std::setprecision(10) << std::fixed << result << std::endl;

    // write dump:
    db.dump();

    return 0;
}
