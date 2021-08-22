#include <iostream>
#include <string>
#include <memory>
using namespace std;

// interface
class IHTML {
public:
    virtual string toString() {
        return script_; 
    }
protected:
    virtual void create_script() = 0;
    string script_ = "Empty";
};

// component
class Text final : public IHTML {
public:
    explicit Text(string text) : text_(text) { 
        create_script(); 
    };
    ~Text() = default;   
    void create_script() override {
        script_ = text_;
    }
private:
    string text_;  // 데이터 중복 저장은 신경쓰지 않는다.
};

class Image final : public IHTML {
public:
    Image(string path, int w, int h) : path_(path), width_(to_string(w)), height_(to_string(h)) { 
        create_script(); 
    };
    ~Image() = default;
    void create_script() override {
        script_ = "<img src=" + path_ + " width=" + width_ + " height=" + height_ + ">";
    }
private:
    string path_;
    string width_;
    string height_;
};

// decorator
class IWrapperTag : public IHTML {
protected:
    string getStartTag() const { return start_script_; }
    string getEndTag() const { return end_script_; }
    void setStartTag(const string s) { start_script_ = s; }
    void setEndTag(const string s) { end_script_ = s; }
private:
    string start_script_;
    string end_script_;
};

class Table final : public IWrapperTag {
public:
    Table(unique_ptr<IHTML> html, int r, int c) : html_(move(html)), rows_(to_string(r)), cols_(to_string(c)) { create_script(); };
    ~Table() = default;
    void create_script() override {
        setStartTag("<table rows=" + rows_ + " cols=" + cols_ + ">");
        setEndTag("</table>");
    }
    string toString() override {
        return getStartTag() + html_->toString() + getEndTag();
    }
private:
    string rows_;
    string cols_;
    unique_ptr<IHTML> html_;
};

class Link final : public IWrapperTag {
public:
    Link(unique_ptr<IHTML> html, string path) : html_(move(html)), path_(path) { create_script(); };
    ~Link() = default;
    void create_script() override {
        setStartTag("<a href=" + path_ + ">");
        setEndTag("</a>");
    }
    string toString() override {
        return getStartTag() + html_->toString() + getEndTag();
    }
private:
    string path_;
    unique_ptr<IHTML> html_;
};

class Font final : public IWrapperTag {
public:
    Font(unique_ptr<IHTML> html, int size) : html_(move(html)), size_(to_string(size)) { create_script(); };
    ~Font() = default;
    void create_script() override {
        setStartTag("<font size=" + size_ + ">");
        setEndTag("</font>");
    }
    string toString() override {
        return getStartTag() + html_->toString() + getEndTag();
    }
private:
    string size_;
    unique_ptr<IHTML> html_;
};

void TEST_1() {
    unique_ptr<IHTML> my_html = 
        make_unique<Table>(
            make_unique<Font>(
                make_unique<Text>("http://www.google.com")
                , 5)
            , 3, 2);
    cout << my_html->toString() << endl;
}

void TEST_2() {
    unique_ptr<IHTML> my_html = make_unique<Image>("logo.jpg", 640, 480);
    my_html = make_unique<Font>(move(my_html), 2);
    my_html = make_unique<Link>(move(my_html), "http://www.design-pattern.com");
    my_html = make_unique<Table>(move(my_html), 2, 2);
    cout << my_html->toString() << endl;
}

int main() {
    TEST_1();
    TEST_2();
    return 0;
}