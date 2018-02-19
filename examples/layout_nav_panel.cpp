#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <forward_list>

static const char LAYOUT[] =
//"<main_ margin=2 grid=[2,1] gap=2>"
//"<left_>|70%<right_>"
//"<left_><right_ weight=70%>"
"vert margin=5"
"<header_ weight=20 margin=[0,30%]>"
"<content_0>"
"<content_1>"
"<content_2>"
"<footer_ weight=20>"
;

static const char* LINKS[] = {
    "<color=0x0080FF target=\"content_0\">Home</>",
    "<color=0x0080FF target=\"content_1\">Test</>",
    "<color=0x0080FF target=\"content_2\">About</>"
};

struct Form : nana::form
{
    Form(nana::rectangle rect, const char* title, unsigned bg) : nana::form(rect)
    {
        caption(title);
        bgcolor(nana::color_rgb(bg));
    }
};

struct Panel : nana::panel<true>
{
    nana::place place{*this};
    Panel(nana::widget& owner, const char* layout) : nana::panel<true>(owner)
    {
        place.div(layout);
    }
};

struct App
{
    std::forward_list<nana::label> links;
    
    Form fm{ {273, 0, 1005, 710}, "Layout example", 0xFFFFFF };
    nana::place place{ fm };
    nana::label bottom{ fm, "Copyright 2018 <color=0x0080FF>David Yu</>" };
    nana::label c1{ fm, "c1" };
    nana::label c2{ fm, "c2" };
    Panel c0 {fm, "<left_><right_>"};
    nana::label left{ c0, "left" };
    nana::label right { c0, "right" };
    
    int current_selected = 0;
    std::string current_target = "content_0";
    
    void links$$(nana::label::command cmd, const std::string& target)
    {
        int selected = target.back() - 48;
        if (nana::label::command::click != cmd || selected == current_selected)
            return;
        
        // hide current
        place.field_display(current_target.c_str(), false);
        
        // set current
        current_selected = selected;
        current_target[current_target.size() - 1] = target.back();
        
        // show
        place.field_display(target.c_str(), true);
        place.collocate();
    }
    
    int show()
    {
        place.div(LAYOUT);
        
        c0.place["left_"] << left;
        c0.place["right_"] << right;
        
        place["content_0"] << c0;
        place["content_1"] << c1;
        place["content_2"] << c2;
        
        auto listener = [this](nana::label::command cmd, const std::string& target) {
            this->links$$(cmd, target);
        };
                
        for (auto text : LINKS)
        {
            links.emplace_front(fm.handle());
            auto& link = links.front();
            
            link.text_align(nana::align::center)
                .format(true)
                .add_format_listener(listener)
                .caption(text);
            
            place["header_"] << link;
        }
        
        bottom.text_align(nana::align::center)
            .format(true);
        place["footer_"] << bottom;
        
        place.field_display("content_1", false);
        place.field_display("content_2", false);
        place.collocate();
        c0.place.collocate();
        
        fm.show();
        nana::exec();
        
        return 0;
    }
};

int main(int argc, char* argv[])
{
    App app;
    return app.show();
}
