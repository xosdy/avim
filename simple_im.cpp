
#include <string>
#include <unistd.h>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/make_shared.hpp>

#include <avtcpif.hpp>
#include <avproto.hpp>

#include <openssl/x509.h>
#include <openssl/pem.h>

// 一个非常非常简单的 IM 实现，测试用途

static boost::asio::io_service io_service;

avkernel avcore(io_service);

int main(int argv, char * argc[])
{
	// TODO, 从 文件加载 RSA key
	// TODO, 加载 ~/.avim/id_rsa.key 和 ~/.avim/id_rsa.cert
	RSA * rsa_key = NULL;
	X509 * x509_cert = NULL;


	// 连接到 im.avplayer.org:24950

	boost::asio::ip::tcp::resolver resolver(io_service);

	boost::shared_ptr<boost::asio::ip::tcp::socket> avserver( new boost::asio::ip::tcp::socket(io_service));

	// 连接到 im.avplayer.org:24950
	boost::asio::connect(*avserver, resolver.resolve(boost::asio::ip::tcp::resolver::query("im.avplayer.org", "24950")));

	// TODO 修正登录过程

	std::string me_addr = "test@avplayer.org";

	// 构造 avtcpif
	// 创建一个 tcp 的 avif 设备，然后添加进去, TODO, 证书也应该传进去
	boost::shared_ptr<avtcpif> avinterface;

	avinterface.reset(new avtcpif(avserver, me_addr, rsa_key, x509_cert) );

	avinterface->slave_handshake(0);

	avcore.add_interface(avinterface);

	// 添加路由表
	avcore.add_route(".+@.+", me_addr, avinterface->get_ifname());

	// 进入 IM 过程，发送一个 test  到 test2@avplayer.org
	avcore.sendto("test2@avplayer.org", "test");

	io_service.run();
}
