#pragma once

#include <terminalpp/core.hpp>

#include <functional>

struct fake_channel
{
    //* =================================================================
    /// \brief Asynchronously read from the channel and call the function
    /// back when it's available.
    //* =================================================================
    void async_read(std::function<void(terminalpp::bytes)> const &callback)
    {
        read_callback = callback;
    }

    //* =================================================================
    /// \brief Write the given data to the channel.
    //* =================================================================
    void write(terminalpp::bytes data)
    {
        written.append(data.begin(), data.end());
    }

    //* =================================================================
    /// \brief Returns whether the channel is alive.
    //* =================================================================
    [[nodiscard]] bool is_alive() const
    {
        return alive;
    }

    //* =================================================================
    /// \brief Closes the channel.
    //* =================================================================
    void close()
    {
        alive = false;

        if (read_callback)
        {
            read_callback({});
        }
    }

    //* =================================================================
    /// \brief Fakes receiving the passed data.
    //* =================================================================
    void receive(terminalpp::bytes data)
    {
        std::function<void(terminalpp::bytes)> callback;
        std::swap(read_callback, callback);

        if (callback)
        {
            callback(data);
        }
    }

    std::function<void(terminalpp::bytes)> read_callback;
    terminalpp::byte_storage written;
    bool alive{true};
};