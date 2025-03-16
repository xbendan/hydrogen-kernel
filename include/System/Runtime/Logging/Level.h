#pragma once

namespace System::Runtime::Logging {
    enum class Level
    {
        /**
         * @brief Records all messages.
         *
         * Includes:
         * - Debug
         * - Information
         * - Detailed messages
         * - Everything else
         */
        Verbose,
        /**
         * @brief Records debug messages.
         */
        Debug,
        /**
         * @brief Records information during properly running.
         *
         * Includes:
         * - User actions
         * - System events
         * - Informational messages
         */
        Information,
        /**
         * @brief Records warnings.
         *
         * Warning messages are typically for non-critical issues. It is
         * recommended to fix these issues, because it means unexpected
         * behavior. However, the system can still run properly.
         *
         * Includes:
         * - Non-critical issues
         * - Unusual events
         */
        Warning,
        /**
         * @brief Records errors.
         *
         * Error messages are for critical issues. One or more system components
         * may not work properly. Issues should be fixed as soon as possible.
         * However, it still won't cause a system crash.
         *
         * Includes:
         * - Critical issues
         * - Unexpected behavior
         */
        Error,
        /**
         * @brief Records fatal errors.
         *
         * Fatal error are for critical issues. The system cannot continue to
         * run, once a fatal error occurs, the system will be immediately
         * shutdown, to prevent further damage.
         *
         * Includes:
         * - Critical issues
         * - Unexpected behavior
         * - System crash
         */
        Fatal
    };
}