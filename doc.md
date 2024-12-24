<span>
  <h1 style="text-decoration: underline solid 1px; text-align:center;font-size: 4rem;">Report On Internship</h1>
  <br>
  <h2 style="text-align:center;font-size: 2rem;">Brainwave Matrix Solutions</h2>
</span>

<div style="text-align: center; margin-top: 2rem;">
  <p style="font-size: 1.5rem;">INTERN:</p>
  <h3 style="font-size: 2rem; font-weight: bold;">Aryan Chauhan</h3>
</div>

<div style="text-align: center; margin-top: 1.5rem;">
  <p style="font-size: 1.5rem;">INTERN FOR:</p>
  <h3 style="font-size: 2rem; font-weight: bold;">C++ Developer</h3>
</div>

<div style="text-align: center; margin-top: 2rem;">
  <p style="font-size: 1.5rem;">Tasks Performed:</p>
  <ul style="list-style-type: none; font-size: 1.5rem; margin: 0; padding: 0;">
    <li>a) ATM System</li>
    <li>b) Banking System</li>
  </ul>
</div>

<div style="margin-top: 2rem; text-align: center;">
  <p style="font-size: 1.5rem;">Date of Submission:</p>
  <h3 style="font-size: 2rem; font-weight: bold;">24 December, 2024</h3>
</div>


<div style="margin-top: 8rem; text-align: center;">
    <h2 id="ack">Acknowledgment</h2>
    <p>
        I would like to express my gratitude to Brainwave Matrix Solutions for providing me with the opportunity to intern as a C++ Developer. The experience of working independently on challenging tasks has greatly contributed to my growth and understanding of software development.
        This internship allowed me to explore, analyze, and solve problems on my own, fostering self-reliance and critical thinking. I am thankful for the trust placed in me to handle these responsibilities and for the resources provided to support my work.
    </p>
</div>

<div style="margin-top: 8rem;">
    <h2>Table Of Contents</h2>
    <ol>
       <li style="font-weight: bold; font-size: 1.2rem;"><a href="#ack" style="text-decorations: none; color: white;">Acknowledgment</a></li>
       <li style="font-weight: bold; font-size: 1.2rem;"><a href="#over" style="text-decorations: none; color: white;">Overview</a></li>
       <li style="font-weight: bold; font-size: 1.2rem;"><a href="#notes" style="text-decorations: none; color: white;">Some Important Notes</a></li>
       <li style="font-weight: bold; font-size: 1.2rem;"><a href="#common_things" style="text-decorations: none; color: white;">Common Things</a></li>
    </ol>
</div>

<div style="margin-top: 8rem;">
    <h2 id="over">Overview</h2>
    <p>
        As part of the internship program launched by <b>Brainwave Matrix Solutions</b>, I would first like to congratulate everyone who participated, including myself. 
        The program was a rewarding experience, as it provided an opportunity to work on meaningful tasks and enhance my skills. During this internship, I worked as a C++ intern on two projects: <b>an ATM System</b> and <b>a Banking System</b>.
    </p>
    <p>
        The purpose of this document is to provide comprehensive documentation of both projects, detailing their functionality, limitations, potential improvements, and the challenges encountered.
    </p>
    <p>
        Before proceeding, here are some important notes.
    </p>
</div>


<div style="margin-top: 8rem;">
  <!--
    Important Notes Section
  -->
  <h2 id="notes">Some Important Notes</h2>
  <p>
    Below are important notes to consider before using the program. These notes outline the requirements, limitations, and discrepancies in the project.
  </p>
  <ul>
    <li>
      <b>Environment Used:</b>
      <p>
        The tasks were completed in a <b>Linux</b> environment using <b>Visual Studio Code</b> as the IDE and <b>GNU GCC</b> as the compiler. The <b>Fedora</b> distribution, specifically <b>Fedora 41</b> (the latest at the time of completion), was used as the operating system. The compiler version was <b>GCC 14.2.1 (20240912)</b>, and the projects were built using the <b>C++23</b> standard.
      </p>
    </li>
    <li>
      <b>Testing Environment:</b>
      <p>
        Testing was performed exclusively on the Fedora distribution of Linux. The programs are guaranteed to work on this platform. Although not explicitly tested, the code is expected to support the Windows environment and should compile and run there as well.
      </p>
    </li>
    <li>
      <b>Dependencies Needed:</b>
      <p>
        For Windows environments, platform-specific headers are the only dependencies required. On Linux, no additional dependencies are needed, as all necessary headers are expected to be available by default in most distributions.
      </p>
    </li>
    <li>
      <b>Extra Software Needed:</b>
      <p>
        Ensure that <b>MAKE</b> is installed. Using the latest version is recommended for optimal compatibility.
      </p>
    </li>
    <li>
      <b>Steps to Use the Program:</b>
      <p>
        Follow these steps to clone and build the project:
        <br><br>
        <b>Clone the repository:</b>
        <br>
        <code>
          git clone --depth=1 https://github.com/MegrajChauhan/Brainwave_Matrix_Intern.git
        </code>
        <br><br>
        <b>Build the project:</b>
        Each project has a dedicated Makefile.
        <br>
        <code>
          cd PROJECT/
          <br>
          make all
        </code>
        <br><br>
        After building, a folder named <b>build</b> will be created, containing the compiled program. Simply run the program from this folder.
      </p>
    </li>
    <li>
      <b>Additional Notes:</b>
      <p>
        Do not modify, change, or delete the contents of the <b>users</b> folder.
      </p>
    </li>
  </ul>
</div>

<div style="margin-top: 8rem;">
  <h2 id="common_things">Common Features</h2>
  <p>
    Both tasks share approximately 80% of their functionality, making it simpler to describe them collectively. Below are the shared components and features:
  </p>
  
  <h3>The Basic Structure</h3>
  <p>
    The basic structure of both tasks is identical, with the <b>Banking System</b> having an additional <b>Server</b> component. In fact, the same code was reused instead of creating a shared library, as the complete code was required for each task. Understanding this structure is essential, and the core structure includes:
  </p>
  <ul>
    <li>
      <b>Frontend:</b><br>
      The frontend handles console interactions, rendering tasks, and making requests to the backend for data. It processes commands, communicates with the backend, retrieves necessary information, and displays it in a user-friendly format.
    </li>
    <li>
      <b>Backend:</b><br>
      The backend is responsible for handling user data, including fetching, creating, updating, and deleting information. It is designed to be modular, allowing it to be replaced with a production-ready implementation. The frontend interacts with the backend solely through an <b>API</b>, making the system flexible.
      <br><br>
      For the Banking System, the backend communicates with the server to retrieve required information.
    </li>
  </ul>
  
  <h3>Commands</h3>
  <p>
    Both programs share the same command set, with the Banking System including an additional command for user registration. Here is the list of commands available in both projects:
  </p>
  <ul>
    <li>
      <b>Help:</b> 
      Displays a help message listing all available commands with brief descriptions and usage instructions. It also provides guidance on navigating the programs.
    </li>
    <li>
      <b>Login:</b>
      Prompts the user to enter their account number and PIN to log in.
    </li>
    <li>
      <b>Logout:</b>
      Logs the user out of their account, allowing another account to log in.
    </li>
    <li>
      <b>Status:</b>
      Shows detailed account information and transaction history.
    </li>
    <li>
      <b>Balance:</b>
      Displays the current balance of the logged-in user.
    </li>
    <li>
      <b>Withdraw:</b>
      Enables the user to withdraw a specified amount from their account.
    </li>
    <li>
      <b>Deposit:</b>
      Allows the user to deposit money into their account.
    </li>
    <li>
      <b>Transfer:</b>
      Facilitates money transfers to other accounts.
    </li>
    <li>
      <b>Clear:</b>
      Clears the console screen for better visibility.
    </li>
    <li>
      <b>Register (Bank Exclusive):</b>
      Registers a new user and generates a new account number.
    </li>
    <li>
      <b>Quit:</b>
      Exits the program.
    </li>
  </ul>
<h3>Control Flow</h3>
<p>
  Both programs share a similar control flow for the most part. The workflow of each program can be broadly categorized into the following components:
</p>
<ul>
  <li>
    <b>Console:</b><br>
    This component handles rendering text to the screen and managing the viewport, including support for scrolling. The console is set to raw mode before the rest of the system is initialized.
  </li>
  <li>
    <b>IO:</b><br>
    The IO component manages user input and buffers past inputs as "history." Its primary role is to handle input requests. It also ensures secure PIN entry by hiding the entered characters.
  </li>
  <li>
    <b>Session:</b><br>
    The session component acts as the central manager, bringing everything together. It initializes the API and implements all supported commands. It interacts with the API to fetch and utilize user information as needed.
  </li>
  <li>
    <b>API:</b><br>
    The API serves as an interface between the backend and the frontend (i.e., the Session). For example, if the program were adapted for public use, the same API could be used with a more sophisticated backend. Additionally, the API maintains the state of the currently logged-in user.
  </li>
</ul>
<br>
<p>
  <b>Additional Component (Banking System Exclusive):</b><br>
  <b>Server and Client:</b><br>
  These components are specific to the banking system. The <b>Server</b> listens for connections on port 8080 and functions as the backend. The API communicates with the server through the <b>Client</b>.
</p>
  <h3>Features</h3>
  <li>Has a dynamic CLI which adapts to the changes in the screen size.</li>
  <li>Allows scrolling dynamic behavior with the console and hides PINs.</li>
  <li>Stores old commands as inputs including the account numbers and excluding the PINs.</li>
  <li>Has a very basic but flexible error handling with proper error messages.</li>
  <li>Uses a custom format to store user information in directories.</li>
  <li>Has a lot of commands and new ones can be easily added.</li>
  <li>Flexiblity with the backend.</li>
  <li>Basic security in place.</li>
  <li>Supports 3 different account types.</li>
  <li>Friendly and understandable interface.</li>
  <li>Minimal in terms of work done and features provided.</li>
  <li>Simple navigation system.</li>
  <li>Highly expandable.</li>
  <li>Compatible with each other.</li>

  <h3>Drawbacks</h3>
  <li>Inefficient user fetching and storing.</li>
  <li>Inefficient rendering method if scaled up.</li>
  <li>No logs in case of random crashes.</li>
  <li>No protection in place for the user's data in the directories.</li>
  <li>Not failsafe against the failures from the standard library or the operating system.</li>
  <li>Extremely simple components.</li>
  <li>Extremely simple way of storing user data.</li>
  <li>Doesn't support much of the account types.</li>
  <li>The history system is too risky for a real-world application.</li>

  <h3>Decisions Made</h3>
  <li>
    <b>Database VS File system: </b> Due to a lack of adequate information, limited time, and no responses to emails, I chose to use the file system instead of a database. Given the time constraints and the need to minimize complexity, this decision was made despite the tradeoff in speed. File reads and parsing are slower compared to efficient database lookups, but the simplicity and reduced setup time were more important in this case.
  </li>
  <li>
    <b>Rendering Bottleneck: </b> A key decision was to create a simple, custom CLI without relying on additional dependencies that could introduce bottlenecks. Although optimizations like mimicking a back buffer have been implemented, rendering still exhibits inefficiencies, including a noticeable blink when updating the display.  
  </li>
  <li>
    <b>Server(For Banking System): </b> The server is quite basic and designed to handle multiple clients in theory, but it can only support one client at a time in practice. Rather than performing all actions on the server side, the client informs the server of what occurred, and the server takes the necessary actions. The communication protocol is simple, but this simplicity makes the system vulnerable to attacks.   
  </li>

  <h3>Some Solutions</h3>
  Since it would be unprofessional on my part for not thinking about the solutions, here are some:
  <li>
    Implementing a library such as NCurses would significantly enhance the CLI experience. This would not only simplify the handling of terminal interfaces but also allow for the creation of more sophisticated, user-friendly, and visually appealing interfaces.
  </li>
  <li>
   Transitioning from a file-based storage system to a database would improve speed and efficiency, especially as the system scales. A database would allow faster lookups, better data integrity, and easier management of large datasets.
  </li>
  <li>
    Strengthening the security of user data is critical. This can be done by implementing encryption for sensitive information both in transit and at rest. Additionally, employing secure authentication methods (e.g., two-factor authentication) and using strong password hashing algorithms (e.g., bcrypt, Argon2) will make the system far more secure.
  </ii>
  <li>
   The security of the program itself needs to be bolstered, particularly in the banking system's communication protocol. Implementing encrypted communication channels (e.g., TLS/SSL) for data exchange will prevent man-in-the-middle attacks. Additionally, input validation and authentication protocols should be enforced across all components to reduce vulnerability to external threats.
  </li>
  <li>
    Refactoring the system to be more modular is essential for long-term maintainability and scalability. Adopting a modular approach allows for easier updates, testing, and integration of new features without disrupting the core system.
  </li>
  <li>
     Implementing a robust error logging system would greatly benefit both users and developers. 
  </li>
</div>