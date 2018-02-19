#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <forward_list>

namespace ui {
    
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
        nana::place place{ *this };
        
        Panel(nana::widget& owner, const char* layout) : nana::panel<true>(owner)
        {
            place.div(layout);
        }
    };
    
} // ui

struct Home : ui::Panel
{
    nana::label left{ *this, "left" };
    nana::label right{ *this, "right" };
    
    Home(nana::widget& owner) : Panel(owner, 
        "<left_ weight=30%>"
        "<right_>"
    )
    {
        left.bgcolor(nana::color_rgb(0xFAFAFA));
        
        place["left_"] << left;
        place["right_"] << right;
        place.collocate();
    }
};

static const char* LINKS[] = {
    "<color=0x0080FF size=11 target=\"content_0\">Home</>",
    "<color=0x0080FF size=11 target=\"content_1\">Test</>",
    "<color=0x0080FF size=11 target=\"content_2\">About</>"
};

struct App
{
    std::forward_list<nana::label> links;
    int current_selected{ 0 };
    std::string current_target{ "content_0" };
    
    ui::Form fm{ {273, 0, 1005, 710}, "Layout example", 0xFFFFFF };
    nana::place place{ fm };
    nana::label bottom{ fm, "Copyright 2018 <color=0x0080FF>David Yu</>" };
    ui::Panel content{ fm,
        "vert"
        "<content_0>"
        "<content_1>"
        "<content_2>"
    };
    Home home{ content };
    nana::label test{ content, "c1" };
    nana::label about{ content, "c2" };
    
    App()
    {
        place.div(
            "vert margin=5"
            "<header_ weight=20 margin=[0,30%] arrange=[18]>"
            "<content_>"
            "<footer_ weight=20>"
        );
        
        about.bgcolor(nana::color_rgb(0xFCFCFC));
        
        // content
        content.place["content_0"] << home;
        
        content.place["content_1"] << test;
        content.place.field_display("content_1", false);
        
        content.place["content_2"] << about;
        content.place.field_display("content_2", false);
        
        content.place.collocate();
        place["content_"] << content;
        
        // bottom
        place["footer_"] << bottom.text_align(nana::align::center).format(true);
    }
    
    void links$$(const std::string& target)
    {
        int selected = target.back() - 48;
        if (selected == current_selected)
            return;
        
        // hide current
        content.place.field_display(current_target.c_str(), false);
        
        // set current
        current_selected = selected;
        current_target[current_target.size() - 1] = target.back();
        
        // show
        content.place.field_display(target.c_str(), true);
        content.place.collocate();
    }
    
    int show()
    {
        nana::picture menu(fm);
        
        menu.transparent(true);
        menu.load(nana::paint::image("examples/assets/menu18x18.ico"));
        menu.events().click([this](const nana::arg_click& arg) {
            this->links$$("content_0");
        });
        menu.events().mouse_move([&menu](const nana::arg_mouse& arg) {
            menu.cursor(nana::cursor::hand);
        });
        
        place["header_"] << menu;
        
        // header
        auto listener = [this](nana::label::command cmd, const std::string& target) {
            if (nana::label::command::click == cmd)
                this->links$$(target);
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
        
        place.collocate();
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
