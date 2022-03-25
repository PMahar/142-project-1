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

void Buffer::populate_anchors() {
    for (std::string x : v_lines_) {
        string anchor_content;
        std::stringstream line_stream;
        line_stream << x;
        string line_content;
        int tag_begin = x.find('<');
        int tag_end = x.find('>');
        if (tag_begin != std::string::npos) {
            anchor_content = x.substr(tag_begin, tag_end + 1);
            v_anchors_.push_back(anchor_content);
            if (tag_end + 1 != x.length()) {
                string line_remainder = x.substr(tag_end + 1);
                tag_begin = ((line_remainder).find('<'));
                if (tag_begin != std::string::npos) {
                    tag_end = line_remainder.find('>');
                    anchor_content = line_remainder.substr(tag_begin, tag_end + 1);
                    v_anchors_.push_back(anchor_content);
                }
            }
        }
    }
}

/*void Buffer::print_anchors() {
    for (string x : v_anchors_)
        std::cout << x << endl;
}*/

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


