// Buffer.cpp

#include "Buffer.h"

using namespace std;

void Buffer::display() const
{
    int ix_stop_line_ = ix_top_line_ + window_height_;
    for (int i = ix_top_line_; i < ix_stop_line_; ++i) {
        if (i < v_formatted_lines_.size())
            //cout << std::setw(6) << i + 1 << "  " << v_lines_[i];
            cout << std::setw(6) << i + 1 << "  " << (v_formatted_lines_[i]);
            cout << '\n';
    }
}

bool Buffer::open(const string & new_file_name)
{
    std::ifstream file(new_file_name);
    if (!file)
        return false;

    v_lines_.clear();
    v_formatted_lines_.clear();
    // Note: the vector is cleared only after we know the file opened successfully.

    string line;
    while (getline(file, line))
        v_lines_.push_back(line);

    file_name_ = new_file_name;
    ix_top_line_ = 0;
    return true;
}

bool Buffer::go(const int& link_number) {
//    if (!(isdigit(link_number)) || link_number < 1 || link_number > v_anchors_.size())
//        return false;
    open(v_anchors_[link_number - 1]);
    return true;
}

std::string Buffer::format_anchor(const std::string &anchor_line) { //Store an anchor's linked file, and also return
    stringstream anchor_content;                                        //return the text that should be displayed
    stringstream print_line;
    string reference_file;
    string reference_title;
    anchor_content << anchor_line;
    anchor_content.ignore(); // '<'
    anchor_content.ignore(); //'a'
    anchor_content >> reference_file;
    anchor_content >> reference_title;
    for (int i = 0; i < v_anchors_.size(); i++) { //For duplicate anchors, return their existing index
        if (v_anchors_[i] == reference_file) {
            print_line << "<" << reference_title << "[" << i + 1 << "]";
            return print_line.str();
        }
    }
    v_anchors_.push_back(reference_file);
    print_line << "<" << reference_title << "[" << v_anchors_.size() << "]";
    return print_line.str();
}

void Buffer::format_lines() {
    for (string line: v_lines_) {
        int line_length = 0;
        std::stringstream line_content;
        std::stringstream formatted_line;
        line_content << line;
        string word;
        string line_substr = line_content.str();
        while (line_content >> word) {
            if (word == "<a") {
                string anchor_line;
                int tag_begin = line_substr.find('<');
                int tag_end = line_substr.find('>');
                anchor_line = line_substr.substr(tag_begin);
                string anchor_word = format_anchor(anchor_line);
                line_length += anchor_word.length();
                if (line_length <= window_width_) {
                    formatted_line << anchor_word;
                } else {
                    line_length = 0; //Reset and insert newline
                    formatted_line << anchor_word << "\n";
                }
                line_substr = line_substr.substr(tag_end + 1);  //Search the rest of the line for any more anchors
                line_content >> word; //Ignore the content of an anchor, which gets parsed by format_anchor
                line_content >> word;
            } else {
                line_length += word.length();
                if (line_length <= window_width_) {
                    formatted_line << word << " ";
                } else {
                    line_length = 0;
                    formatted_line << word << "\n" << " ";
                }
            }
        }
        v_formatted_lines_.push_back(formatted_line.str()); //Populate a new vector of formatted lines
    }
}

bool Buffer::search(const string & substring)
{
    for (int i = ix_top_line_; i < v_lines_.size(); i++) {
        if (v_lines_[i].find(substring) != string::npos) {
            ix_top_line_ = i;
            return true;
        }
    }
    return false;
}


