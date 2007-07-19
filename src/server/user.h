/*******************************************************************************

   Copyright (C) 2006, 2007 M.K.A. <wyrmchild@users.sourceforge.net>

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.

*******************************************************************************/

#ifndef ServerUser_INCLUDED
#define ServerUser_INCLUDED

#include "ev/event.h" // event::~

#include "socket.h" // Socket class
#include "message_ref.h" // message_ref
#include "buffer.h" // Buffer
#include "array.h" // Array<>
#include "types.h" // octet, etc.

#include <deque> // std::deque
#include <map> // std::map

struct Socket;
struct SessionData;
struct Session;

namespace protocol
{
	struct Message;
	struct ToolInfo;
}

//! User/Client information
struct User
{
	//! Default constructor
	/**
	 * @param[in] _id User identifier
	 * @param[in] nsock Socket to associate with User
	 */
	User(const octet _id, const Socket& nsock) throw();
	
	//! Destructor
	~User() throw();
	
	//! Change active session
	/**
	 * @param[in] session_id The session which to activate
	 */
	bool makeActive(octet session_id) throw();
	
	//! Fetch SessionData* pointer
	/**
	 * @param[in] session_id Which session to fetch
	 */
	SessionData* getSession(octet session_id) throw();
	
	//! Fetch const SessionData* pointer
	/**
	 * @param[in] session_id Which session to fetch
	 */
	const SessionData* getConstSession(octet session_id) const throw();
	
	//! Cache tool info
	/**
	 * @param[in] ti Tool info message to cache
	 * @throw std::bad_alloc If it can't allocate local copy of the tool info
	 */
	void cacheTool(protocol::ToolInfo* ti) throw(std::bad_alloc);
	
	//! Socket
	Socket sock;
	
	//! Currently active session
	Session *session;
	
	//! User identifier
	uint id;
	
	//! Event I/O : registered events.
	// EventSystem::ev_t // inaccessible for some reason
	event::ev_type<EventSystem>::ev_t events;
	//int events;
	
	//! User state
	enum State
	{
		//! When user has just connected
		Init,
		
		//! User has been verified to be using correct protocol.
		Verified,
		
		//! Waiting for proper user info
		Login,
		
		//! Waiting for password
		LoginAuth,
		
		//! Normal operation
		Active
	} state;
	
	uint
		//! Active layer in session
		layer,
		//! Session we're currently syncing.
		syncing;
	
	//! Is the user server admin?
	bool isAdmin;
	
	//! Client can live with ACKs alone
	bool c_acks;
	
	//! Get client capabilities
	/**
	 * @return Flags for use with the network protocol
	 */
	octet getCapabilities() const throw();
	
	//! Set client capabilities
	/**
	 * @param[in] flags as used in the network protocol
	 */
	void setCapabilities(const octet flags) throw();
	
	bool
		//! Deflate extension
		ext_deflate,
		//! Chat extension
		ext_chat,
		//! Palette extension
		ext_palette;
	
	//! Get extensions
	/**
	 * @return Flags as used in the network protocol
	 */
	octet getExtensions() const throw();
	
	//! Set extensions
	/**
	 * @param[in] flags as used in the network protocol
	 */
	void setExtensions(const octet flags) throw();
	
	//! Subscribed sessions
	std::map<octet, SessionData*> sessions;
	
	//! Output queue
	std::deque<message_ref> queue;
	
	Buffer
		//! Input buffer
		input,
		//! Output buffer
		output;
	
	//! Currently incoming message.
	protocol::Message *inMsg;
	
	//! Feature level used by client
	uint level;
	
	//! Password seed associated with this user.
	char seed[4];
	
	//! Last touched.
	time_t deadtime;
	
	//! User name
	Array<char> name;
	
	//! Active session data
	SessionData* session_data;
	
	//! Stroke counter
	u_long strokes;
	
	//! 'Flushes' queue to output buffer
	uint flushQueue();
};

#endif // ServerUser_INCLUDED
