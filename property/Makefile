#
# (C) Copyright 2004-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LIB	= $(obj)libproperty.o

COBJS-y += cmd_cboot.o
COBJS-y += normal_mode.o
COBJS-y += recv_mode.o
COBJS-y += fastboot_mode.o
COBJS-y += charge_mode.o
COBJS-y += alarm_mode.o
COBJS-y += dloader_mode.o
COBJS-y += android_boot.o
COBJS-y += calibration_detect.o
COBJS-y += engtest_mode.o
COBJS-y += sleep_mode.o
COBJS-y += vbat_check.o
COBJS-y += dcdc_calbration.o
COBJS-y += spl_writer.o



COBJS	:= $(sort $(COBJS-y))
SRCS	:= $(COBJS:.o=.c) $(XCOBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

CPPFLAGS += -I..

all:	$(LIB)

$(LIB): $(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
