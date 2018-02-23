#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>

//Creates a textbox and button
//textbox shows the value of the sub item
//button is used to delete the item.
class inline_widget : public nana::listbox::inline_notifier_interface
{
    inline_indicator * indicator_{ nullptr };
    index_type  pos_ ;
    nana::textbox     txt_ ;
    nana::button      btn_ ;

private:
    //Creates inline widget
    //listbox calls this method to create the widget
    //The position and size of widget can be ignored in this process
    void create(nana::window wd) override
    {
        //Create listbox
        txt_.create(wd);
        txt_.events().click([this]
        {
            //Select the item when clicks the textbox
            indicator_->selected(pos_);
        });

        txt_.events().mouse_move([this]
        {
            //Highlight the item when hovers the textbox
            indicator_->hovered(pos_);
        });

        txt_.events().key_char([this](const nana::arg_keyboard& arg)
        {
            if (arg.key == nana::keyboard::enter)
            {
                //Modify the item when enter is pressed
                arg.ignore = true;
                indicator_->modify(pos_, txt_.caption());
            }
        });

        //Create button
        btn_.create(wd);
        btn_.caption("Delete");
        btn_.events().click([this]
        {
            //Delete the item when button is clicked
            auto & lsbox = dynamic_cast<nana::listbox&>(indicator_->host());
            lsbox.erase(lsbox.at(pos_));
        });

        btn_.events().mouse_move([this]
        {
            //Highlight the item when hovers the button
            indicator_->hovered(pos_);
        });
    }

    //Activates the inline widget, bound to a certain item of the listbox
    //The inline_indicator is an object to operate the listbox item,
    //pos is an index object refers to the item of listbox
    void activate(inline_indicator& ind, index_type pos) override
    {
        indicator_ = &ind;
        pos_ = pos;
    }

    //Notify the item status
    //This method is introduced since 1.4.1
    void notify_status(status_type status, bool status_on) override
    {
        switch(status)
        {
        case status_type::selecting:
            //If status_on is true, the item is selecting.
            //If status on is false, the item is unselecting
            break;
        case status_type::checking:
            //If status_on is true, the item is checking
            //If status_on is false, the item is unchecking
            break;
        }
    }

    //Sets the inline widget size
    //dimension represents the max size can be set
    //The coordinate of inline widget is a logical coordinate to the sub item of listbox
    void resize(const nana::size& dimension) override
    {
        auto sz = dimension;
        sz.width -= 50;
        txt_.size(sz);

        nana::rectangle r(sz.width + 5, 0, 45, sz.height);
        btn_.move(r);
    }

    //Sets the value of inline widget with the value of the sub item
    void set(const value_type& value) override
    {
        txt_.caption(value);
    }

    //Determines whether to draw the value of sub item
    //e.g, when the inline widgets covers the whole background of the sub item,
    //it should return false to avoid listbox useless drawing
    bool whether_to_draw() const override
    {
        return false;
    }
private:
};

static const int WIDTH = 1005,
        HEIGHT = 710,
        MARGIN = 5,
        // listbox
        LB_OUTER = MARGIN * 2,
        LB_HEIGHT = HEIGHT - LB_OUTER,
        LB_WIDTH = WIDTH - LB_OUTER,
        // inner
        LB_FIELDS = 2,
        LB_INNER = MARGIN * 3 * LB_FIELDS,
        OTHER_WIDTH = 25,
        MAIN_WIDTH = LB_WIDTH - LB_INNER - OTHER_WIDTH;

int main()
{
    nana::form fm{ {10, 10, WIDTH, HEIGHT} };
    fm.bgcolor(nana::colors::white);
    
    nana::listbox lsbox{ fm, { MARGIN, MARGIN, LB_WIDTH, LB_HEIGHT } };

    //Create two columns
    lsbox.show_header(false);
    lsbox.checkable(true);
    lsbox.append_header("", OTHER_WIDTH);
    lsbox.append_header("", MAIN_WIDTH);
    
    //Set the inline_widget, the first column of category 0, the second column of category 1
    lsbox.at(0).inline_factory(1, nana::pat::make_factory<inline_widget>());
    
    //Then append items
    lsbox.at(0).append({ "", "Foo" });
    lsbox.at(0).append({ "", "Bar" });
    lsbox.at(0).append({ "", "Hello" });
    lsbox.at(0).append({ "", "World" });
    
    //Create a new category
    //lsbox.append("Category 1");

    //Append items for category 1
    //lsbox.at(1).append({ std::string("Hello4"), std::string("World4") });
    //lsbox.at(1).append({ std::string("Hello5"), std::string("World5") });
    //lsbox.at(1).append({ std::string("Hello6"), std::string("World6") });
    //lsbox.at(1).append({ std::string("Hello7"), std::string("World7") });
    
    fm.show();
    nana::exec();
}
