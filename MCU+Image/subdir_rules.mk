################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-2116374689: ../image.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs1030/ccs/utils/sysconfig_1.8.0/sysconfig_cli.bat" -s "C:/ti/simplelink_cc32xx_sdk_5_10_00_02/.metadata/product.json" --script "C:/Users/stefa/git/pee50/image.syscfg" -o "syscfg" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_drivers_net_wifi_config.json: build-2116374689 ../image.syscfg
syscfg/: build-2116374689

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/stefa/git/pee50" --include_path="C:/Users/stefa/git/pee50/MCU+Image" --include_path="C:/ti/simplelink_cc32xx_sdk_5_10_00_02/source" --include_path="C:/ti/simplelink_cc32xx_sdk_5_10_00_02/kernel/nortos" --include_path="C:/ti/simplelink_cc32xx_sdk_5_10_00_02/kernel/nortos/posix" --include_path="C:/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.4.LTS/include" --define=NORTOS_SUPPORT -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/stefa/git/pee50/MCU+Image/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1125098881: ../pwmled2.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs1030/ccs/utils/sysconfig_1.8.0/sysconfig_cli.bat" -s "C:/ti/simplelink_cc32xx_sdk_5_10_00_02/.metadata/product.json" --script "C:/Users/stefa/git/pee50/pwmled2.syscfg" -o "syscfg" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_drivers_config.c: build-1125098881 ../pwmled2.syscfg
syscfg/ti_drivers_config.h: build-1125098881
syscfg/ti_utils_build_linker.cmd.genlibs: build-1125098881
syscfg/syscfg_c.rov.xs: build-1125098881
syscfg/ti_utils_runtime_model.gv: build-1125098881
syscfg/ti_utils_runtime_Makefile: build-1125098881
syscfg/: build-1125098881

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/stefa/git/pee50" --include_path="C:/Users/stefa/git/pee50/MCU+Image" --include_path="C:/ti/simplelink_cc32xx_sdk_5_10_00_02/source" --include_path="C:/ti/simplelink_cc32xx_sdk_5_10_00_02/kernel/nortos" --include_path="C:/ti/simplelink_cc32xx_sdk_5_10_00_02/kernel/nortos/posix" --include_path="C:/ti/ccs1030/ccs/tools/compiler/ti-cgt-arm_20.2.4.LTS/include" --define=NORTOS_SUPPORT -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Users/stefa/git/pee50/MCU+Image/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


