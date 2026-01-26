#include "core/server/WebServer.hpp"

int	main() {
	struct s_location	s_loca;
	s_loca.route = "/";
	s_loca.root_path = "../../../www/html";
	s_loca.autoindex = true;

	struct s_listen	s_list;
	s_list.ip = "127.0.0.1";
	s_list.port = 8080;

	struct s_server	s_serv;
	s_serv.listen.push_back(s_list);
	s_serv.root = "../../../www/html";
	s_serv.client_max_body_size = 2000;

	Location		location(s_loca);
	std::vector<Location>	v_locs;
	v_locs.push_back(location);

	Server			servConf(s_serv, v_locs);
	std::vector<Server>	v_serv;
	v_serv.push_back(servConf);

	Configuration	config(v_serv);

	WebServer	webserv(config);
	webserv.run();

	return (0);
}
