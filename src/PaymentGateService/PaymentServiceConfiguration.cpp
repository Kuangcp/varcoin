// Copyright (c) 2012-2018, The CryptoNote developers, YxomTech
//
// This file is part of Varcoin.
//
// Varcoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Varcoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Varcoin.  If not, see <http://www.gnu.org/licenses/>.

#include "PaymentServiceConfiguration.h"

#include <iostream>
#include <algorithm>
#include <boost/program_options.hpp>

#include "Logging/ILogger.h"

namespace po = boost::program_options;

namespace PaymentService {

Configuration::Configuration() {
  generateNewWallet = false;
  daemonize = false;
  registerService = false;
  unregisterService = false;
  logFile = "payment_gate.log";
  testnet = false;
  printAddresses = false;
  logLevel = Logging::INFO;
  bindAddress = "";
  bindPort = 0;
}

void Configuration::initOptions(boost::program_options::options_description& desc) {
  desc.add_options()
      ("bind-address", po::value<std::string>()->default_value("0.0.0.0"), "payment service bind address")
      ("bind-port", po::value<uint16_t>()->default_value(8070), "payment service bind port")
      ("wallet-file,w", po::value<std::string>(), "wallet file")
      ("wallet-password,p", po::value<std::string>(), "wallet password")
      ("generate-wallet,g", "generate new wallet file and exit")
      ("daemon,d", "run as daemon in Unix or as service in Windows")
#ifdef _WIN32
      ("register-service", "register service and exit (Windows only)")
      ("unregister-service", "unregister service and exit (Windows only)")
#endif
      ("log-file,l", po::value<std::string>(), "log file")
      ("server-root", po::value<std::string>(), "server root. The service will use it as working directory. Don't set it if don't want to change it")
      ("log-level", po::value<size_t>(), "log level")
      ("address", "print wallet addresses and exit");
}

void Configuration::init(const boost::program_options::variables_map& options) {
  if (options.count("daemon") != 0) {
    daemonize = true;
  }

  if (options.count("register-service") != 0) {
    registerService = true;
  }

  if (options.count("unregister-service") != 0) {
    unregisterService = true;
  }

  if (registerService && unregisterService) {
    throw ConfigurationError("It's impossible to use both \"register-service\" and \"unregister-service\" at the same time");
  }

  if (options.count("testnet") != 0) {
    testnet = true;
  }

  if (options.count("log-file") != 0) {
    logFile = options["log-file"].as<std::string>();
  }

  if (options.count("log-level") != 0) {
    logLevel = options["log-level"].as<size_t>();
    if (logLevel > Logging::TRACE) {
      std::string error = "log-level option must be in " + std::to_string(Logging::FATAL) +  ".." + std::to_string(Logging::TRACE) + " interval";
      throw ConfigurationError(error.c_str());
    }
  }

  if (options.count("server-root") != 0) {
    serverRoot = options["server-root"].as<std::string>();
  }

  if (options.count("bind-address") != 0 && (!options["bind-address"].defaulted() || bindAddress.empty())) {
    bindAddress = options["bind-address"].as<std::string>();
  }

  if (options.count("bind-port") != 0 && (!options["bind-port"].defaulted() || bindPort == 0)) {
    bindPort = options["bind-port"].as<uint16_t>();
  }

  if (options.count("wallet-file") != 0) {
    walletFile = options["wallet-file"].as<std::string>();
  }

  if (options.count("wallet-password") != 0) {
    walletPassword = options["wallet-password"].as<std::string>();
  }

  if (options.count("generate-wallet") != 0) {
    generateNewWallet = true;
  }

  if (options.count("address") != 0) {
    printAddresses = true;
  }

  if (!registerService && !unregisterService) {
    if (walletFile.empty() || walletPassword.empty()) {
      throw ConfigurationError("Both wallet-file and wallet-password parameters are required");
    }
  }
}

} //namespace PaymentService
