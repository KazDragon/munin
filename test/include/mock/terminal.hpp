#pragma once
#include <terminalpp/terminal.hpp>
#include <iosfwd>
#include <gmock/gmock.h>

class mock_terminal : public terminalpp::terminal
{
public :
    //* =====================================================================
    /// \brief Returns a string that will initialize the terminal.
    //* =====================================================================
    MOCK_METHOD0(init, std::string ());

    //* =====================================================================
    /// \brief Returns a string that will enable the best possible mouse
    /// mode for the terminal.
    //* =====================================================================
    MOCK_METHOD0(enable_mouse, std::string ());

    //* =====================================================================
    /// \brief Returns a string that will disable the mouse.
    //* =====================================================================
    MOCK_METHOD0(disable_mouse, std::string ());

    //* =====================================================================
    /// \brief Returns a string that will set the window title of the
    /// terminal.
    //* =====================================================================
    MOCK_METHOD1(set_window_title, std::string (std::string const &title));

    //* =====================================================================
    /// \brief Sets the (local) size of the terminal.
    /// There is nothing transmitted by this.  This is so that the client
    /// can set how large the terminal is expected to be.  This affects
    /// things like when does a cursor scroll the screen, etc.
    //* =====================================================================
    MOCK_METHOD1(set_size, void (terminalpp::extent const &size));

    //* =====================================================================
    /// \brief Show the cursor.
    //* =====================================================================
    MOCK_METHOD0(show_cursor, std::string ());

    //* =====================================================================
    /// \brief Hide the cursor.
    //* =====================================================================
    MOCK_METHOD0(hide_cursor, std::string ());

    //* =====================================================================
    /// \brief Saves the current cursor position.
    //* =====================================================================
    MOCK_METHOD0(save_cursor, std::string ());

    //* =====================================================================
    /// \brief Restores the previously saved cursor position.
    //* =====================================================================
    MOCK_METHOD0(restore_cursor, std::string ());

    //* =====================================================================
    /// \brief Move the cursor to the specified position.  Note: terminals are
    /// 1-based.  I.e. the origin position is (1,1).
    //* =====================================================================
    MOCK_METHOD1(move_cursor, std::string (terminalpp::point const &pt));

    //* =====================================================================
    /// \brief Reads a stream of data.
    //* =====================================================================
    MOCK_METHOD1(read, std::vector<terminalpp::token> (std::string const &data));

    //* =====================================================================
    /// \brief Writes the specified element.
    //* =====================================================================
    MOCK_METHOD1(write, std::string (terminalpp::element const &elem));

    //* =====================================================================
    /// \brief Writes the specified sequence of characters.
    //* =====================================================================
    MOCK_METHOD1(write, std::string (terminalpp::string const &str));

    //* =====================================================================
    /// \brief Erases the display in the specified manner.
    //* =====================================================================
    MOCK_METHOD1(erase_in_display, std::string (erase_display how));

    //* =====================================================================
    /// \brief Erases the current line in the specified manner.
    //* =====================================================================
    MOCK_METHOD1(erase_in_line, std::string (erase_line how));

    //* =====================================================================
    /// \brief Sets the terminal to the normal screen buffer (default).
    //* =====================================================================
    MOCK_METHOD0(use_normal_screen_buffer, std::string ());

    //* =====================================================================
    /// \brief Sets the terminal to the alternate screen buffer.
    //* =====================================================================
    MOCK_METHOD0(use_alternate_screen_buffer, std::string ());
};

// TODO: put these in Terminal++
namespace terminalpp {

inline std::ostream &operator<<(std::ostream &out, virtual_key const &vk)
{
    return out << "vk"; 
}

namespace ansi { 
    
inline std::ostream &operator<<(std::ostream &out, control_sequence const &cseq)
{
    return out << "cseq";
}

namespace mouse {

inline std::ostream &operator<<(std::ostream &out, report const &rep)
{
    return out << "mouse";
}

}}}
