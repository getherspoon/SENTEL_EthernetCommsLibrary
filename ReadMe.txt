Ethernet Comms Module
=====================

This module is responsible for all common ethernet communications tools.  What this means is that any ethernet comms
class, utility, etc should be housed in this module so long as the tool is generic and not specific to any one purpose
or device.  An example of a tool that does not belong would be a Video Server class.  Though this class uses ethernet comms,
it does not belong in this module.  Rather, it should use the comms from this module to generate the server comms, but it should
elsewhere in the solution.

The goal of this module is to house easy to use, and completely reusable tools for UDP, TCP, Multicast, and so forth.

All networking code should make use of the boost::asio library. This should always be the case unless there is a strong and valid reason not to. 
If such a case arises, it should be discussed with the team prior to commitment.

This module's purpose is to aid in future development of ethernet devices and interfaces by providing simple and minimal tools which allow
for a small learning curve and easy development.

Please see the Readme-[Folder].txt within each sub folder of this Module as starting point. Then see the comments within each file for more information.

NOTE: All files contained in the root "Header Files" and "Source Files" folders relate directly to the module itself and not to the any actual tool.
      All other files should be separated by purpose in folders.
