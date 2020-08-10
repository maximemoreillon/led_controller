// Head partials
// Style
const String styles = "<style>"
  "body{}"
  "nav a:not(:last-child) {margin-right: 0.5em;}"
  "footer {margin-top: 1em;}"
  "</style>";

// Body partials
const String header = "<header>"
  "  <h1>LED controller</h1>"
  "</header>";

const String nav = "<nav class='nav'>"
  "<a href='/'>Status</a>"
  "<a href='/color_form'>Color selection</a>"
  "<a href='/update_form'>Firmware update</a>"
  "</nav>";
  
const String footer= "<footer>"
  "  <div>LED controller firmware v0.5</div>"
  "  <div>Maxime Moreillon</div>"
  "</footer>";

// Update form
String update_form = "<h2>Firmware update</h2>"
  "<form method='POST' action='/update' enctype='multipart/form-data'>"
  "<input type='file' name='update'>"
  "<input type='submit' value='Update'>"
  "</form>";

// Combinations
String head ="<head>"
  "<title>" + String(HOSTNAME) +"</title>"
  + styles +
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "</head>";

String pre_main = "<html>"
  + head + 
  "<body>"
  + header + nav +
  "<main>";
  
String post_main = "</main>"
  + footer +
  "</body>"
  "</html>";

// Root
String root_main = "<h2>Status</h2>"
  "<p>Hostname: " + String(HOSTNAME) + "</p>";

String color_form = "<h2>Color selection</h2>"
  "<form action='/color' method='get'>"
  "  <label>Color:</label>"
  "  <input type='color' name='color' value='#ff0000'><br>"
  "  <label>White</label>"
  "  <input type='range' name='w' min='0' max='1023'><br>"
  "  <input type='submit'>"
  "</form>";

// Not found
String not_found = "<h2>Not found</h2>";
