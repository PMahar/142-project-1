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
    string reference_title;
    string reference_file;
    anchor_content << anchor_line;
    anchor_content.ignore(); // '<'
    anchor_content.ignore(); //'a'
    anchor_content >> reference_title;
    anchor_content >> reference_file;
    v_anchors_.push_back(reference_file);
    return reference_title;
}

void Buffer::format_display() {
    for (string l: v_lines_) {
        std::stringstream line_content;
        line_content << l;
        string word;
        while (line_content >> word) {
            if (word == "<a") {
                //string reference_title = populate_anchors(word);
                //cout << "[" << reference_title << "]" << v_anchors_.size() + 1;
                string anchor_content;
                int tag_begin = line_content.str().find('<');
                int tag_end = line_content.str().find('>');
                anchor_content = line_content.str().substr(tag_begin, tag_end);
                cout << "<" << populate_anchors(anchor_content) << ">" << "[" << v_anchors_.size() << "] ";
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


