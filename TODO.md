# TODO

The following is a list of thing I have yet to implement

- ~~Publish to GitHub~~
- Send static files
- Generate dictionary of available request body data
  - i.e. turn `foo=bar&baz` into `{{"foo", "bar"},{"baz", "1"}}`
- Send populated templates
- Automate `Content-Type` where possible
- `typedef std::vector<std::pair<std::string, typename T>> dictionary<typename T>`
- Implement specifying required/optional arguments
  - i.e. things like `...{"baz", "0"}...` can exist if `baz` isn't there.
- Dispatch commands to send output from them instead.
  - Add warning, since starting a process for this purpose is a terrible idea.
- Send JSON from C++ types
  - `std::vector<T>` > `[...]`
  - `std::vector<std::pair<std::string, T>> aka cgi::dictionary<T>` > `{...: ...}`
  - `int` > `...`
- Use of SQLite
- Automatic authentication
- WebSockets?!?!?!?!??
- PLUGINS???!!!!?!?!?!??!?!??!??!?!!!?
- *document :(*
- ...
- Profit

