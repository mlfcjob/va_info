#include <stdio.h>
#include <stdlib.h>
#include <va/va_x11.h>
#include <va/va.h>


#define CHECK_VASTATUS(va_status,func, ret)                             \
if (va_status != VA_STATUS_SUCCESS) {                                   \
    fprintf(stderr,"%s failed with error code %d (%s),exit\n",func, va_status, vaErrorStr(va_status)); \
    ret_val = ret;                                                      \
    goto error;                                                         \
}

static int encode_support(VAProfile profile)
{
    switch (profile) {
            case VAProfileNone:
                break;
            case VAProfileMPEG2Simple:
            case VAProfileMPEG2Main:
            case VAProfileMPEG4Simple:
            case VAProfileMPEG4AdvancedSimple:
            case VAProfileMPEG4Main:
                break;
            case VAProfileH264Baseline:
                printf("h264 Baseline.\n");
                break;
            case VAProfileH264Main:
                printf("h264 Main.\n");
                break;
            case VAProfileH264High:
                printf("h264 High.\n");
                break;
            case VAProfileH264ConstrainedBaseline:
                printf("h264 Constrained Baseline.\n");
                break;
            case VAProfileH264MultiviewHigh:
                printf("h264 Mulitve High.\n");
                break;
            case VAProfileH264StereoHigh:
                printf("h264 StereoHigh.\n");
                break;
            case VAProfileVC1Simple:
            case VAProfileVC1Main:
            case VAProfileVC1Advanced:
                break;
            case VAProfileH263Baseline:
                break;
            case VAProfileJPEGBaseline:
                break;
            case VAProfileVP8Version0_3:
                break;
            case VAProfileHEVCMain:
                printf("HEVC Main.\n");
                break;
            case VAProfileHEVCMain10:
                printf("HEVC Main10.\n");
                break;
            case VAProfileVP9Profile0:
            case VAProfileVP9Profile1:
            case VAProfileVP9Profile2:
            case VAProfileVP9Profile3:
                break;
            defualt:
                break;
    }

    return 0;
}

static char * profile_string(VAProfile profile)
{
    switch (profile) {
            case VAProfileNone: return "VAProfileNone";
            case VAProfileMPEG2Simple: return "VAProfileMPEG2Simple";
            case VAProfileMPEG2Main: return "VAProfileMPEG2Main";
            case VAProfileMPEG4Simple: return "VAProfileMPEG4Simple";
            case VAProfileMPEG4AdvancedSimple: return "VAProfileMPEG4AdvancedSimple";
            case VAProfileMPEG4Main: return "VAProfileMPEG4Main";
            case VAProfileH264Baseline: return "VAProfileH264Baseline";
            case VAProfileH264Main: return "VAProfileH264Main";
            case VAProfileH264High: return "VAProfileH264High";
            case VAProfileVC1Simple: return "VAProfileVC1Simple";
            case VAProfileVC1Main: return "VAProfileVC1Main";
            case VAProfileVC1Advanced: return "VAProfileVC1Advanced";
            case VAProfileH263Baseline: return "VAProfileH263Baseline";
            case VAProfileH264ConstrainedBaseline: return "VAProfileH264ConstrainedBaseline";
            case VAProfileJPEGBaseline: return "VAProfileJPEGBaseline";
            case VAProfileVP8Version0_3: return "VAProfileVP8Version0_3";
            case VAProfileH264MultiviewHigh: return "VAProfileH264MultiviewHigh";
            case VAProfileH264StereoHigh: return "VAProfileH264StereoHigh";
            case VAProfileHEVCMain: return "VAProfileHEVCMain";
            case VAProfileHEVCMain10: return "VAProfileHEVCMain10";
            case VAProfileVP9Profile0: return "VAProfileVP9Profile0";
            case VAProfileVP9Profile1: return "VAProfileVP9Profile1";
            case VAProfileVP9Profile2: return "VAProfileVP9Profile2";
            case VAProfileVP9Profile3: return "VAProfileVP9Profile3";

            default:
                break;
    }
    return "<unknown profile>";
}

static VADisplay *x11_display;
void va_close_display_x11(VADisplay va_dpy)
{

    if (!x11_display)
        return;

    XCloseDisplay(x11_display);
    x11_display = NULL;

}

static VADisplay va_open_display_x11(void)
{
    x11_display = XOpenDisplay(NULL);
    if (!x11_display) {
       fprintf(stderr, "error: can't connect X server!\n");\
       return NULL;
    }
    return vaGetDisplay(x11_display);
}

VADisplay va_open_display(void)
{
    VADisplay va_dpy = NULL;

    va_dpy =  va_open_display_x11();

    if (!va_dpy) {
        fprintf(stderr, "error failed to initialize display.\n");
        abort();
    }
    return va_dpy;
}

int main(int argc, char *argv[])
{

    VADisplay va_dpy;
    VAStatus va_status;
    int major_version, minor_version;
    VAProfile *profile_list = NULL;
    int num_profiles, max_num_profiles, i;
    int ret_val = 0;

    va_dpy = va_open_display();
    if (NULL == va_dpy) {
        fprintf(stderr, "vaGetDisplay() failed.\n");
        return 2;
    }

    va_status  = vaInitialize(va_dpy, &major_version, &minor_version);
    CHECK_VASTATUS(va_status, "vaInitialize", 3);

    printf("VA-API version: %d.%d (libva) \n", major_version, minor_version);
    
    max_num_profiles = vaMaxNumProfiles(va_dpy);
    profile_list = malloc(max_num_profiles * sizeof(VAProfile));

    if (!profile_list) {
       printf("Failed to allocate memory for profile list.\n");
       ret_val = 5;
       goto error;
    }

    va_status = vaQueryConfigProfiles(va_dpy, profile_list, &num_profiles);

    for (i = 0; i < num_profiles; i++) {
        char *profile_str;
        profile_str = profile_string(profile_list[i]);
        printf("****%s.\n", profile_str);
        //encode_support(profile_list[i]);
    }

error:
    free(profile_list);
    vaTerminate(va_dpy);
    va_close_display_x11(va_dpy);


    return ret_val;
}


