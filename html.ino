// Head partials
// Style
const String styles = ""
  "<style>"
    "body{}"
    "nav a:not(:last-child) {margin-right: 0.5em;}"
    "footer {margin-top: 1em;}"
  "</style>";

// Body partials
const String header = ""
  "<header>"
    "<h1>LED controller</h1>"
  "</header>";

const String nav = ""
  "<nav class='nav'>"
    "<a href='/'>Home</a>"
    "<a href='/color'>Color selection</a>"
    "<a href='/update'>Firmware update</a>"
  "</nav>";
  
const String footer= ""
  "<footer>"
    "<div>LED controller</div>"
    "<div>Maxime Moreillon</div>"
  "</footer>";

String color_subheader = ""
  "<h2>Color selection</h2>";

String firmware_update_subheader = ""
  "<h2>Firmware update</h2>";
  
// Update form
String update_form = ""
  +firmware_update_subheader+
  "<form method='POST' action='/update' enctype='multipart/form-data'>"
    "<input type='file' name='update'>"
    "<input type='submit' value='Update'>"
  "</form>";

// Combinations
String head = ""
  "<head>"
    "<title>LED controller | " + String(HOSTNAME) +"</title>"
    + styles +
    "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "</head>";

String pre_main = ""
  "<html>"
    + head + 
    "<body>"
      + header + nav +
      "<main>";
  
String post_main = ""
      "</main>"
      + footer +
    "</body>"
  "</html>";

// Root
String root_main(){

  //time_t current_time = DateTime.now();
  
  return ""
  "<h2>Home</h2>"
  "<p>Hostname: " + String(HOSTNAME) + "</p>"
  "<p>Firmware: v" + String(FIRMWARE_VERSION) + "</p>"
  "<p>"
    "Uptime: " +uptime_formatter::getUptime()+ ""
  "</p>"
  "<p><a href='/on'>Turn on</a></p>"
  "<p><a href='/off'>Turn off</a></p>";
}


  
String color_form(){
  return ""
  +color_subheader+
  "<form action='/color' method='POST'>"
    "<p>"
      "<label>Color</label>"
      "<input type='color' name='color' value='#"+color_to_string()+"'>"
    "</p>"
    "<p>"
      "<label>White</label>"
      "<input type='range' name='w' min='0' value='"+String(W_channel.duty_when_on)+"' max='"+String(PWM_MAX_DUTY)+"'><br>"
    "</p>"
    "<p>"
      "<input type='submit'>"
    "</p>"
  "</form>";
}

// Not found
String not_found = "<h2>Not found</h2>";
