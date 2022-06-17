/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2022 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_CHAT_BOX_HPP
#define TGUI_CHAT_BOX_HPP


#include <TGUI/CopiedSharedPtr.hpp>
#include <TGUI/Widgets/Scrollbar.hpp>
#include <TGUI/Renderers/ChatBoxRenderer.hpp>
#include <TGUI/Text.hpp>
#include <deque>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TGUI_API ChatBox : public Widget
    {
    public:

        typedef std::shared_ptr<ChatBox> Ptr; //!< Shared widget pointer
        typedef std::shared_ptr<const ChatBox> ConstPtr; //!< Shared constant widget pointer

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        struct Line
        {
            Text text;
            String string;
        };


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Constructor
        /// @param typeName     Type of the widget
        /// @param initRenderer Should the renderer be initialized? Should be true unless a derived class initializes it.
        /// @see create
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ChatBox(const char* typeName = "ChatBox", bool initRenderer = true);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Creates a new chat box widget
        ///
        /// @return The new chat box
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static ChatBox::Ptr create();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Makes a copy of another chat box
        ///
        /// @param chatBox  The other chat box
        ///
        /// @return The new chat box
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static ChatBox::Ptr copy(ChatBox::ConstPtr chatBox);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the renderer, which gives access to functions that determine how the widget is displayed
        /// @return Temporary pointer to the renderer that may be shared with other widgets using the same renderer
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ChatBoxRenderer* getSharedRenderer();
        const ChatBoxRenderer* getSharedRenderer() const;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the renderer, which gives access to functions that determine how the widget is displayed
        /// @return Temporary pointer to the renderer
        /// @warning After calling this function, the widget has its own copy of the renderer and it will no longer be shared.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ChatBoxRenderer* getRenderer();
        const ChatBoxRenderer* getRenderer() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the size of the chat box
        ///
        /// This size does not include the borders.
        ///
        /// @param size   The new size of the chat box
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setSize(const Layout2d& size) override;
        using Widget::setSize;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Adds a new line of text to the chat box
        ///
        /// The whole text passed to this function will be considered as one line for the getLine and removeLine functions,
        /// even if it is too long and gets split over multiple lines.
        ///
        /// The default text color and style will be used.
        ///
        /// @param text  Text that will be added to the chat box
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void addLine(const String& text);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Adds a new line of text to the chat box
        ///
        /// The whole text passed to this function will be considered as one line for the getLine and removeLine functions,
        /// even if it is too long and gets split over multiple lines.
        ///
        /// The default text style will be used.
        ///
        /// @param text   Text that will be added to the chat box
        /// @param color  Color of the text
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void addLine(const String& text, Color color);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Adds a new line of text to the chat box
        ///
        /// The whole text passed to this function will be considered as one line for the getLine and removeLine functions,
        /// even if it is too long and gets split over multiple lines.
        ///
        /// @param text   Text that will be added to the chat box
        /// @param color  Color of the text
        /// @param style  Text style
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void addLine(const String& text, Color color, TextStyles style);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the contents of the requested line
        ///
        /// @param lineIndex  The index of the line of which you request the contents
        ///                   The first line has index 0
        ///
        /// @return The contents of the requested line
        ///         An empty string will be returned when the index is too high
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        String getLine(std::size_t lineIndex) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the color of the requested line
        ///
        /// @param lineIndex  The index of the line of which you request the color. The first line has index 0
        ///
        /// @return The color of the requested line. The default color (set with setTextColor) when the index is too high.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Color getLineColor(std::size_t lineIndex) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the text style of the requested line
        /// @param lineIndex  The index of the line of which you request the text style. The first line has index 0.
        /// @return The text style of the requested line. The default style (set with setTextStyle) when the index is too high.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TextStyles getLineTextStyle(std::size_t lineIndex) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Removes the requested line
        ///
        /// @param lineIndex  The index of the line that should be removed
        ///                   The first line has index 0
        ///
        /// @return True if the line was removed, false if no such line existed (index too high)
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool removeLine(std::size_t lineIndex);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Removes all lines from the chat box
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void removeAllLines();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the amount of lines in the chat box
        ///
        /// @return Number of lines in the chat box
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::size_t getLineAmount();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets a maximum amount of lines in the chat box
        ///
        /// Only the last maxLines lines will be kept. Lines above those will be removed.
        /// Set to 0 to disable the line limit (default).
        ///
        /// @param maxLines  The maximum amount of lines that the chat box can contain
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setLineLimit(std::size_t maxLines);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the maximum amount of lines in the chat box
        ///
        /// Only the last maxLines lines will be kept. Lines above those will be removed.
        /// Disabled when set to 0 (default).
        ///
        /// @return The maximum amount of lines that the chat box can contain
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::size_t getLineLimit();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the default color of the text
        /// @param color  The new default text color
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextColor(Color color);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the default color of the text
        /// @return The currently used default text color
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const Color& getTextColor() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the default text style
        /// @param style  The new default text style
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextStyle(TextStyles style);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the default text style
        /// @return The currently used default text style
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TextStyles getTextStyle() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Lets the first lines start from the top or from the bottom of the chat box
        ///
        /// Note that this only makes a difference when the lines don't fill the entire chat box.
        /// This does not change the order of the lines.
        ///
        /// @param startFromTop  Let the first lines be placed at the top of the chat box, or remain at the bottom?
        ///
        /// By default the first lines will be placed at the bottom of the chat box.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setLinesStartFromTop(bool startFromTop = true);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns whether the first lines start from the top or from the bottom of the chat box
        ///
        /// Note that this only makes a difference when the lines don't fill the entire chat box.
        /// This does not change the order of the lines.
        ///
        /// @return Are the first lines displayed at the top of the chat box?
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool getLinesStartFromTop() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets whether new lines are added below the other lines or above them
        ///
        /// @param newLinesBelowOthers  Should the addLine function insert the line below the existing lines?
        ///
        /// By default the new lines are always added below the others.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setNewLinesBelowOthers(bool newLinesBelowOthers = true);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns whether new lines are added below the other lines or above them
        ///
        /// @return Does the addLine function insert the line below the existing lines?
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool getNewLinesBelowOthers() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the thumb position of the scrollbar
        ///
        /// @param value  New value of the scrollbar
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setScrollbarValue(unsigned int value);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the thumb position of the scrollbar
        ///
        /// @return Value of the scrollbar
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int getScrollbarValue() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns whether the mouse position (which is relative to the parent widget) lies on top of the widget
        ///
        /// @return Is the mouse on top of the widget?
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isMouseOnWidget(Vector2f pos) const override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void leftMousePressed(Vector2f pos) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void leftMouseReleased(Vector2f pos) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void mouseMoved(Vector2f pos) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool mouseWheelScrolled(float delta, Vector2f pos) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void mouseNoLongerOnWidget() override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void leftMouseButtonNoLongerDown() override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    private:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Recalculates the text attribute of the line
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void recalculateLineText(Line& line);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Recalculates all text attributes, recalculate the full text height and update the displayed text
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void recalculateAllLines();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Recalculates the space used by all the lines
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void recalculateFullTextHeight();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Updates the position of the lines
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void updateDisplayedText();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Updates the position and size of the panel and scrollbar
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void updateRendering();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Draw the widget to a render target
        ///
        /// @param target Render target to draw to
        /// @param states Current render states
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void draw(BackendRenderTarget& target, RenderStates states) const override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Function called when one of the properties of the renderer is changed
        ///
        /// @param property  Name of the property that was changed
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void rendererChanged(const String& property) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Saves the widget as a tree node in order to save it to a file
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::unique_ptr<DataIO::Node> save(SavingRenderersMap& renderers) const override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Loads the widget from a tree of nodes
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Called when the text size is changed (either by setTextSize or via the renderer)
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void updateTextSize() override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Returns the size without the borders
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Vector2f getInnerSize() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Makes a copy of the widget
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Widget::Ptr clone() const override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        Color m_textColor = Color::Black;
        TextStyles m_textStyle;

        std::size_t m_maxLines = 0;

        float m_fullTextHeight = 0;

        bool m_linesStartFromTop = false;
        bool m_newLinesBelowOthers = true;

        CopiedSharedPtr<ScrollbarChildWidget> m_scroll;

        std::deque<Line> m_lines;

        Sprite m_spriteBackground;

        // Cached renderer properties
        Borders m_bordersCached;
        Padding m_paddingCached;
        Color   m_backgroundColorCached;
        Color   m_borderColorCached;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_TEXT_BOX_HPP
