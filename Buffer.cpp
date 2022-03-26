// Buffer.cpp

#include "Buffer.h"

using namespace std;

void Buffer::display() const
{
    int ix_stop_line_ = ix_top_line_ + window_height_;
    for (int i = ix_top_line_; i < ix_stop_line_; ++i) {
        if (i < v_lines_.size())
            cout << std::setw(6) << i + 1 << "  " << v_lines_[i];
        cout << '\n';
    }
}

bool Buffer::open(const string & new_file_name)
{
    std::ifstream file(new_file_name);
    if (!file)
        return false;

    v_lines_.clear();
    // Note: the vector is cleared only after we know the file opened successfully.

    string line;
    while (getline(file, line))
        v_lines_.push_back(line);

    file_name_ = new_file_name;
    ix_top_line_ = 0;
    return true;
}

std::string Buffer::populate_anchors(const std::string &anchor_line) {
    stringstream anchor_content;
    stringstream print_line;
    string reference_file;
    string reference_title;
    anchor_content << anchor_line;
    anchor_content.ignore(); // '<'
    anchor_content.ignore(); //'a'
    anchor_content >> reference_file;
    anchor_content >> reference_title;
    for (int i = 0; i < v_anchors_.size(); i++) {
        if (v_anchors_[i] == reference_file) {
            print_line << reference_title << "[" << i + 1<< "]";
            return print_line.str();
        }
    }
    v_anchors_.push_back(reference_file);
    print_line << reference_title << "[" << v_anchors_.size() << "]";
    return print_line.str();
}

bool Buffer::go(int link_number) {
    if (!(isdigit(link_number)) || link_number < 1 || link_number > v_anchors_.size())
        return false;
    open(v_anchors_[link_number + 1]);
    return true;
}

void Buffer::format() {
    for (const string& line : v_lines_) {
        std::stringstream line_content;
        line_content << line;
        string word;
        string line_substr = line_content.str();
        while (line_content >> word) {
            if (word == "<a") {
                string anchor_line;
                int tag_begin = line_substr.find('<');
                int tag_end = line_substr.find('>');
                anchor_line = line_substr.substr(tag_begin);
                cout << "<" << populate_anchors(anchor_line);
                line_substr = line_substr.substr(tag_end + 1);
                line_content >> word;
                line_content >> word;
            } else {
                cout << word;
                cout << ' ';
            }
        }
        cout << endl;
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


