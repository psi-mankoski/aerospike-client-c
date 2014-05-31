/******************************************************************************
 *	Copyright 2008-2013 by Aerospike.
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy 
 *	of this software and associated documentation files (the "Software"), to 
 *	deal in the Software without restriction, including without limitation the 
 *	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
 *	sell copies of the Software, and to permit persons to whom the Software is 
 *	furnished to do so, subject to the following conditions:
 *	
 *	The above copyright notice and this permission notice shall be included in 
 *	all copies or substantial portions of the Software.
 *	
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 *	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *	IN THE SOFTWARE.
 *****************************************************************************/

#pragma once 

#include <aerospike/as_error.h>
#include <aerospike/as_policy.h>

/******************************************************************************
 *	MACROS
 *****************************************************************************/

#ifdef __linux__
/**
 * Default path to the system UDF files.
 */
#define AS_CONFIG_LUA_SYSTEM_PATH "/opt/aerospike/client/sys/udf/lua"

/**
 * Default path to the user UDF files.
 */
#define AS_CONFIG_LUA_USER_PATH "/opt/aerospike/client/usr/udf/lua"
#endif

#ifdef __APPLE__
/**
 * Default path to the system UDF files.
 */
#define AS_CONFIG_LUA_SYSTEM_PATH "/usr/local/aerospike/client/sys/udf/lua"

/**
 * Default path to the user UDF files.
 */
#define AS_CONFIG_LUA_USER_PATH "/usr/local/aerospike/client/usr/udf/lua"
#endif

/**
 * The size of path strings
 */
#define AS_CONFIG_PATH_MAX_SIZE 256

/**
 * The maximum string length of path strings
 */
#define AS_CONFIG_PATH_MAX_LEN 	(AS_CONFIG_PATH_MAX_SIZE - 1)

/**
 * The size of as_config.hosts
 */
#define AS_CONFIG_HOSTS_SIZE 256

/******************************************************************************
 *	TYPES
 *****************************************************************************/

/**
 *	Host Information
 *
 *	@ingroup as_config_object
 */
typedef struct as_config_host_s {
	
	/**
	 *	Host address
	 */
	const char * addr;
	
	/**
	 *	Host port
	 */
	uint16_t port;

} as_config_host;

/**
 *	IP translation table.
 *
 *	@ingroup as_config_object
 */
typedef struct as_addr_map_s {
	
	/**
	 *	Original hostname or IP address in string format.
	 */
    char * orig;
	
	/**
	 *	Use this IP address instead.
	 */
    char * alt;
	
} as_addr_map;

/**
 *	lua module config
 *
 *	@ingroup as_config_object
 */
typedef struct as_config_lua_s {

	/**
	 *	Enable caching of UDF files in the client
	 *	application.
	 */
	bool cache_enabled;

	/**
	 *	The path to the system UDF files. These UDF files 
	 *	are installed with the aerospike client library.
	 *	Default location defined in: AS_CONFIG_LUA_SYSTEM_PATH
	 */
	char system_path[AS_CONFIG_PATH_MAX_SIZE];

	/**
	 *	The path to user's UDF files.
	 *	Default location defined in: AS_CONFIG_LUA_USER_PATH
	 */
	char user_path[AS_CONFIG_PATH_MAX_SIZE];

} as_config_lua;

/**
 *	The `as_config` contains the settings for the `aerospike` client. Including
 *	default policies, seed hosts in the cluster and other settings.
 *
 *	## Initialization
 *
 *	Before using as_config, you must first initialize it. This will setup the 
 *	default values.
 *
 *	~~~~~~~~~~{.c}
 *	as_config config;
 *	as_config_init(&config);
 *	~~~~~~~~~~
 *
 *	Once initialized, you can populate the values.
 *
 *	## Seed Hosts
 *	
 *	The client will require at least one seed host defined in the 
 *	configuration. The seed host is defined in `as_config.hosts`. 
 *
 *	~~~~~~~~~~{.c}
 *	config.hosts[0] = { .addr = "127.0.0.1", .port = 3000 };
 *	~~~~~~~~~~
 *
 *	You can define up to 16 hosts for the seed. The client will iterate over 
 *	the list until it connects with one of the hosts. 
 *
 *	## Policies
 *
 *	The configuration also defines default policies for the application. The 
 *	`as_config_init()` function already presets default values for the policies.
 *	
 *	Policies define the behavior of the client, which can be global across
 *	operations, global to a single operation, or local to a single use of an
 *	operation.
 *	
 *	Each database operation accepts a policy for that operation as an a argument.
 *	This is considered a local policy, and is a single use policy. This policy
 *	supersedes any global policy defined.
 *	
 *	If a value of the policy is not defined, then the rule is to fallback to the
 *	global policy for that operation. If the global policy for that operation is
 *	undefined, then the global default value will be used.
 *
 *	If you find that you have behavior that you want every use of an operation
 *	to utilize, then you can specify the default policy in as_config.policies.
 *
 *	For example, the `aerospike_key_put()` operation takes an `as_policy_write`
 *	policy. If you find yourself setting the `key` policy value for every call 
 *	to `aerospike_key_put()`, then you may find it beneficial to set the global
 *	`as_policy_write` in `as_policies.write`, which all write operations will use.
 *
 *	~~~~~~~~~~{.c}
 *	config.policies.write.key = AS_POLICY_KEY_SEND;
 *	~~~~~~~~~~
 *
 *	If you find that you want to use a policy value across all operations, then 
 *	you may find it beneficial to set the default policy value for that policy 
 *	value.
 *
 *	For example, if you keep setting the key policy value to 
 *	`AS_POLICY_KEY_SEND`, then you may want to just set `as_policies.key`. This
 *	will set the global default value for the policy value. So, if an global
 *  operation policy or a local operation policy does not define a value, then
 *	this value will be used.
 *
 *	~~~~~~~~~~{.c}
 *	config.policies.key = AS_POLICY_KEY_SEND;
 *	~~~~~~~~~~
 *
 *	Global default policy values:
 *	-	as_policies.timeout
 *	-	as_policies.retry
 *	-	as_policies.key
 *	-	as_policies.gen
 *	-	as_policies.exists
 *
 *	Global operation policies:
 *	-	as_policies.read
 *	-	as_policies.write
 *	-	as_policies.operate
 *	-	as_policies.remove
 *	-	as_policies.query
 *	-	as_policies.scan
 *	-	as_policies.info
 *
 *
 *	## User-Defined Function Settings
 *	
 *	If you are using using user-defined functions (UDF) for processing query 
 *	results (i.e aggregations), then you will find it useful to set the 
 *	`mod_lua` settings. Of particular importance is the `mod_lua.user_path`, 
 *	which allows you to define a path to where the client library will look for
 *	Lua files for processing.
 *	
 *	~~~~~~~~~~{.c}
 *	strcpy(config.mod_lua.user_path, "/home/me/lua");
 *	~~~~~~~~~~
 *
 *	@ingroup client_objects
 */
typedef struct as_config_s {

	/**
	 *	A IP translation table is used in cases where different clients use different server
	 *	IP addresses.  This may be necessary when using clients from both inside and outside
	 *	a local area network.  Default is no translation.
	 *
	 *	The key is the IP address returned from friend info requests to other servers.  The
	 *	value is the real IP address used to connect to the server.
	 *
	 *	A deep copy of ip_map is performed in aerospike_connect().  The caller is
	 *  responsible for memory deallocation of the original data structure.
	 */
	as_addr_map * ip_map;
	
	/**
	 *	Length of ip_map array.
	 *  Default: 0
	 */
	uint32_t ip_map_size;
	
	/**
	 *	Estimate of incoming threads concurrently using synchronous methods in the client instance.
	 *	This field is used to size the synchronous connection pool for each server node.
	 *	Default: 300
	 */
	uint32_t max_threads;
	
	/**
	 *	@private
	 *	Not currently used.
	 *	Maximum socket idle in seconds.  Socket connection pools will discard sockets
	 *	that have been idle longer than the maximum.
	 *	Default: 14
	 */
	uint32_t max_socket_idle_sec;
	
	/**
	 *	Initial host connection timeout in milliseconds.  The timeout when opening a connection
	 *	to the server host for the first time.
	 *	Default: 1000
	 */
	uint32_t conn_timeout_ms;

	/**
	 *	Polling interval in milliseconds for cluster tender
	 *	Default: 1000
	 */
	uint32_t tender_interval;

	/**
	 *	Count of entries in hosts array.
	 */
	uint32_t hosts_size;
	
	/**
	 *	(seed) hosts
	 *	Populate with one or more hosts in the cluster
	 *	that you intend to connect with.
	 */
	as_config_host hosts[AS_CONFIG_HOSTS_SIZE];

	/**
	 *	Client policies
	 */
	as_policies policies;

	/**
	 *	lua config
	 */
	as_config_lua lua;
	
} as_config;

/******************************************************************************
 *	FUNCTIONS
 *****************************************************************************/

/**
 *	Initialize the configuration to default values.
 *
 *	You should do this to ensure the configuration has valid values, before 
 *	populating it with custom options.
 *
 *	~~~~~~~~~~{.c}
 *		as_config config;
 *		as_config_init(&config);
 *		as_config_add_host(&config, "127.0.0.1", 3000);
 *	~~~~~~~~~~
 *	
 *	@param c The configuration to initialize.
 *	
 *	@return The initialized configuration on success. Otherwise NULL.
 *
 *	@relates as_config
 */
as_config * as_config_init(as_config * c);

/**
 *	Add host to seed the cluster.
 *
 *	~~~~~~~~~~{.c}
 *		as_config config;
 *		as_config_init(&config);
 *		as_config_add_host(&config, "127.0.0.1", 3000);
 *	~~~~~~~~~~
 *
 *	@relates as_config
 */
static inline void
as_config_add_host(as_config* config, const char* addr, uint16_t port)
{
	as_config_host* host = &config->hosts[config->hosts_size++];
	host->addr = addr;
	host->port = port;
}
