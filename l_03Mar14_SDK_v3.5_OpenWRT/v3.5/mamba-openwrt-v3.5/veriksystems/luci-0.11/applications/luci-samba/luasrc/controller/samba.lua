--[[
LuCI - Lua Configuration Interface

Copyright 2008 Steven Barth <steven@midlink.org>
Copyright 2008 Jo-Philipp Wich <xm@leipzig.freifunk.net>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

$Id: samba.lua 9558 2012-12-18 13:58:22Z jow $
]]--

module("luci.controller.samba", package.seeall)

function index()
	if not nixio.fs.access("/etc/config/samba") then
		return
	end

	local page

	page = entry({"admin", "services", "samba"}, cbi("samba"), _("Network Shares"))
	page.dependent = true
end
