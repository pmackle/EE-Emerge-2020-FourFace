################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Source/%.obj: ../Source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-msp430_20.2.1.LTS/bin/cl430" -vmsp -O3 --use_hw_mpy=none --include_path="C:/ti/ccs1000/ccs/ccs_base/msp430/include" --include_path="C:/Users/noaht/Google Drive/School/2019-2020/ee-emerge/FaceFirmware - CCS (OLD FROM Github)/EE-Emerge-2020-FourFace/Software/FaceFirmware" --include_path="C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-msp430_20.2.1.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Source/$(basename $(<F)).d_raw" --obj_directory="Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


