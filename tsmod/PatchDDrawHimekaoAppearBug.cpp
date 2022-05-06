#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSMod.h"


int iBudhouNodOfPathDDrawHimekaoApeearBug = -1;
extern int iExterniFaceIdOffGeneralKaoFaceExecuteNormalizedFaceID;

// そもそもちゃんと顔が出ない番号。よってこれらの番号は使ってはならない
DDRAW_HIMEKAO_BUG_MAPPING nb6ddraw_himekao_bug[10] = {
    { 49392,    49360 },  // [姫一]の[０ベースで７][目１][口１] →  [姫一]の[０ベースで６][目１][口１]
    { 51825,    51793 },  // [姫二]の[０ベースで３][目２][口２] →  [姫二]の[０ベースで２][目２][口２] 
    { 52146,    52114 },  // [姫三]の[０ベースで１３][目２][口２] →   [姫三]の[０ベースで１２][目２][口２]
    { 52306,    52274 },  // [姫三]の[０ベースで２][目３][口２] →   [姫三]の[０ベースで１][目３][口２]
    { 52913,    52945 },  // [姫二]の[０ベースで５][目４][口２] →  [姫二]の[０ベースで６][目４][口２] 
    { 53040,    53072 },  // [姫一]の[０ベースで９][目４][口２] →   [姫一]の[０ベースで10][目４][口２]
    { 54450,    54418 },  // [姫三]の[０ベースで５][目３][口３] →   [姫三]の[０ベースで４][目３][口３] 
    { 55185,    55217 },  // [姫二]の[０ベースで12][目４][口２] →   [姫二]の[０ベースで13][目４][口２]
    { 55282,    55250 },  // [姫二]の[０ベースで15][目４][口３] →   [姫二]の[０ベースで14][目４][口３]
    { 57170,    57202 },  // [姫二]の[０ベースで10][目４][口４] →   [姫二]の[０ベースで11][目４][口４]
};

// 姫のダメな顔を矯正する。姫以外で使っていてもダメ。
void ModifyWrongFaceDamegao(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		for ( int i=0; i < sizeof(nb6ddraw_himekao_bug)/sizeof(nb6ddraw_himekao_bug[0]); i++ ) {
			// もしダメ顔と一致したなら、もっとも近い矯正顔に直す
			if ( nb6bushouname[iBushouID].number == nb6ddraw_himekao_bug[i].wrong ) {
				nb6bushouname[iBushouID].number = nb6ddraw_himekao_bug[i].right;
				break;
			}
		}
	}
}


// 顔番号→姫bmpの対応
DDRAW_HIMEKAO_MAPPING nb6ddraw_himekao_map[] = {
    { 49168,    3001 },
    { 49169,    3257 },
    { 49170,    3513 },
    { 49200,    3005 },
    { 49201,    3261 },
    { 49202,    3517 },
    { 49232,    3009 },
    { 49233,    3265 },
    { 49234,    3521 },
    { 49264,    3013 },
    { 49265,    3269 },
    { 49266,    3525 },
    { 49296,    3002 },
    { 49297,    3195 },
    { 49298,    3514 },
    { 49328,    3006 },
    { 49329,    3262 },
    { 49330,    3518 },
    { 49360,    3010 },
    { 49361,    3266 },
    { 49362,    3522 },
    { 49393,    3270 },
    { 49394,    3526 },
    { 49424,    3003 },
    { 49425,    3259 },
    { 49426,    3515 },
    { 49456,    3007 },
    { 49457,    3263 },
    { 49458,    3519 },
    { 49488,    3011 },
    { 49489,    3267 },
    { 49490,    3523 },
    { 49520,    3015 },
    { 49521,    3271 },
    { 49522,    3527 },
    { 49552,    3004 },
    { 49553,    3260 },
    { 49554,    3516 },
    { 49584,    3008 },
    { 49585,    3264 },
    { 49586,    3520 },
    { 49616,    3012 },
    { 49617,    3268 },
    { 49618,    3524 },
    { 49648,    3016 },
    { 49649,    3272 },
    { 49650,    3528 },
    { 49680,    3017 },
    { 49681,    3139 },
    { 49682,    3529 },
    { 49712,    3021 },
    { 49713,    3277 },
    { 49714,    3533 },
    { 49744,    3025 },
    { 49745,    3281 },
    { 49746,    3537 },
    { 49776,    3029 },
    { 49777,    3285 },
    { 49778,    3541 },
    { 49808,    3018 },
    { 49809,    3274 },
    { 49810,    3530 },
    { 49840,    3022 },
    { 49841,    3278 },
    { 49842,    3534 },
    { 49872,    3026 },
    { 49873,    3282 },
    { 49874,    3538 },
    { 49904,    3030 },
    { 49905,    3286 },
    { 49906,    3542 },
    { 49936,    3019 },
    { 49937,    3275 },
    { 49938,    3531 },
    { 49968,    3023 },
    { 49969,    3191 },
    { 49970,    3535 },
    { 50000,    3027 },
    { 50001,    3283 },
    { 50002,    3539 },
    { 50032,    3031 },
    { 50033,    3287 },
    { 50034,    3376 },
    { 50064,    3020 },
    { 50065,    3276 },
    { 50066,    3532 },
    { 50096,    3024 },
    { 50097,    3280 },
    { 50098,    3536 },
    { 50128,    3028 },
    { 50129,    3284 },
    { 50130,    3026 },
    { 50160,    3032 },
    { 50161,    3288 },
    { 50162,    3544 },
    { 50192,    3033 },
    { 50193,    3289 },
    { 50194,    3545 },
    { 50224,    3037 },
    { 50225,    3293 },
    { 50226,    3549 },
    { 50256,    3041 },
    { 50257,    3297 },
    { 50258,    3553 },
    { 50288,    3045 },
    { 50289,    3301 },
    { 50290,    3557 },
    { 50320,    3034 },
    { 50321,    3290 },
    { 50322,    3546 },
    { 50352,    3038 },
    { 50353,    3294 },
    { 50354,    3550 },
    { 50384,    3042 },
    { 50385,    3298 },
    { 50386,    3554 },
    { 50416,    3046 },
    { 50417,    3248 },
    { 50418,    3558 },
    { 50448,    3035 },
    { 50449,    3291 },
    { 50450,    3547 },
    { 50480,    3039 },
    { 50481,    3295 },
    { 50482,    3551 },
    { 50512,    3043 },
    { 50513,    3299 },
    { 50514,    3555 },
    { 50544,    3047 },
    { 50545,    3303 },
    { 50546,    3559 },
    { 50576,    3036 },
    { 50577,    3292 },
    { 50578,    3548 },
    { 50608,    3040 },
    { 50609,    3296 },
    { 50610,    3552 },
    { 50640,    3044 },
    { 50641,    3300 },
    { 50642,    3110 },
    { 50672,    3048 },
    { 50673,    3005 },
    { 50674,    3560 },
    { 50704,    3025 },
    { 50705,    3305 },
    { 50706,    3561 },
    { 50736,    3053 },
    { 50737,    3309 },
    { 50738,    3565 },
    { 50768,    3057 },
    { 50769,    3313 },
    { 50770,    3569 },
    { 50800,    3061 },
    { 50801,    3317 },
    { 50802,    3573 },
    { 50832,    3026 },
    { 50833,    3306 },
    { 50834,    3562 },
    { 50864,    3054 },
    { 50865,    3310 },
    { 50866,    3566 },
    { 50896,    3058 },
    { 50897,    3314 },
    { 50898,    3095 },
    { 50928,    3062 },
    { 50929,    3318 },
    { 50930,    3110 },
    { 50960,    3027 },
    { 50961,    3307 },
    { 50962,    3563 },
    { 50992,    3055 },
    { 50993,    3311 },
    { 50994,    3567 },
    { 51024,    3059 },
    { 51025,    3315 },
    { 51026,    3571 },
    { 51056,    3063 },
    { 51057,    3319 },
    { 51058,    3575 },
    { 51088,    3028 },
    { 51089,    3308 },
    { 51090,    3564 },
    { 51120,    3056 },
    { 51121,    3312 },
    { 51122,    3568 },
    { 51152,    3060 },
    { 51153,    3316 },
    { 51154,    3572 },
    { 51184,    3064 },
    { 51185,    3320 },
    { 51186,    3554 },
    { 51216,    3065 },
    { 51217,    3321 },
    { 51218,    3577 },
    { 51248,    3069 },
    { 51249,    3325 },
    { 51250,    3581 },
    { 51280,    3073 },
    { 51281,    3329 },
    { 51282,    3585 },
    { 51312,    3077 },
    { 51313,    3333 },
    { 51314,    3589 },
    { 51344,    3066 },
    { 51345,    3322 },
    { 51346,    3578 },
    { 51376,    3070 },
    { 51377,    3326 },
    { 51378,    3262 },
    { 51408,    3074 },
    { 51409,    3330 },
    { 51410,    3238 },
    { 51440,    3078 },
    { 51441,    3334 },
    { 51442,    3223 },
    { 51472,    3067 },
    { 51473,    3323 },
    { 51474,    3579 },
    { 51504,    3071 },
    { 51505,    3327 },
    { 51506,    3583 },
    { 51536,    3075 },
    { 51537,    3331 },
    { 51538,    3587 },
    { 51568,    3079 },
    { 51569,    3335 },
    { 51570,    3092 },
    { 51600,    3068 },
    { 51601,    3324 },
    { 51602,    3580 },
    { 51632,    3072 },
    { 51633,    3328 },
    { 51634,    3025 },
    { 51664,    3076 },
    { 51665,    3160 },
    { 51666,    3588 },
    { 51696,    3080 },
    { 51697,    3336 },
    { 51698,    3592 },
    { 51728,    3081 },
    { 51729,    3186 },
    { 51730,    3593 },
    { 51760,    3085 },
    { 51761,    3341 },
    { 51762,    3597 },
    { 51792,    3089 },
    { 51793,    3173 },
    { 51794,    3601 },
    { 51824,    3093 },
    { 51826,    3605 },
    { 51856,    3082 },
    { 51857,    3112 },
    { 51858,    3594 },
    { 51888,    3086 },
    { 51889,    3342 },
    { 51890,    3598 },
    { 51920,    3090 },
    { 51921,    3346 },
    { 51922,    3602 },
    { 51952,    3094 },
    { 51953,    3350 },
    { 51954,    3606 },
    { 51984,    3083 },
    { 51985,    3339 },
    { 51986,    3232 },
    { 52016,    3087 },
    { 52017,    3343 },
    { 52018,    3599 },
    { 52048,    3091 },
    { 52049,    3347 },
    { 52050,    3603 },
    { 52080,    3095 },
    { 52081,    3351 },
    { 52082,    3607 },
    { 52112,    3084 },
    { 52113,    3340 },
    { 52114,    3596 },
    { 52144,    3088 },
    { 52145,    3344 },
    { 52176,    3092 },
    { 52177,    3348 },
    { 52178,    3604 },
    { 52208,    3096 },
    { 52209,    3352 },
    { 52210,    3608 },
    { 52240,    3097 },
    { 52241,    3353 },
    { 52242,    3609 },
    { 52272,    3101 },
    { 52273,    3357 },
    { 52274,    3613 },
    { 52304,    3105 },
    { 52305,    3361 },
    { 52336,    3109 },
    { 52337,    3365 },
    { 52338,    3621 },
    { 52368,    3098 },
    { 52369,    3354 },
    { 52370,    3610 },
    { 52400,    3102 },
    { 52401,    3358 },
    { 52402,    3614 },
    { 52432,    3106 },
    { 52433,    3362 },
    { 52434,    3618 },
    { 52464,    3110 },
    { 52465,    3366 },
    { 52466,    3622 },
    { 52496,    3099 },
    { 52497,    3355 },
    { 52498,    3611 },
    { 52528,    3103 },
    { 52529,    3359 },
    { 52530,    3615 },
    { 52560,    3107 },
    { 52561,    3056 },
    { 52562,    3619 },
    { 52592,    3111 },
    { 52593,    3367 },
    { 52594,    3623 },
    { 52624,    3100 },
    { 52625,    3356 },
    { 52626,    3612 },
    { 52656,    3104 },
    { 52657,    3360 },
    { 52658,    3616 },
    { 52688,    3108 },
    { 52689,    3364 },
    { 52690,    3176 },
    { 52720,    3112 },
    { 52721,    3368 },
    { 52722,    3624 },
    { 52752,    3089 },
    { 52753,    3369 },
    { 52754,    3335 },
    { 52784,    3117 },
    { 52785,    3373 },
    { 52786,    3629 },
    { 52816,    3121 },
    { 52817,    3377 },
    { 52818,    3633 },
    { 52848,    3125 },
    { 52849,    3381 },
    { 52850,    3637 },
    { 52880,    3090 },
    { 52881,    3370 },
    { 52882,    3626 },
    { 52912,    3118 },
    { 52914,    3630 },
    { 52944,    3122 },
    { 52945,    3378 },
    { 52946,    3634 },
    { 52976,    3126 },
    { 52977,    3382 },
    { 52978,    3638 },
    { 53008,    3091 },
    { 53009,    3371 },
    { 53010,    3627 },
    { 53041,    3375 },
    { 53042,    3631 },
    { 53072,    3123 },
    { 53073,    3379 },
    { 53074,    3635 },
    { 53104,    3127 },
    { 53105,    3383 },
    { 53106,    3231 },
    { 53136,    3092 },
    { 53137,    3372 },
    { 53138,    3628 },
    { 53168,    3120 },
    { 53169,    3376 },
    { 53170,    3005 },
    { 53200,    3124 },
    { 53201,    3380 },
    { 53202,    3060 },
    { 53232,    3128 },
    { 53233,    3384 },
    { 53234,    3640 },
    { 53264,    3129 },
    { 53265,    3385 },
    { 53266,    3641 },
    { 53296,    3133 },
    { 53297,    3389 },
    { 53298,    3645 },
    { 53328,    3137 },
    { 53329,    3393 },
    { 53330,    3649 },
    { 53360,    3141 },
    { 53361,    3397 },
    { 53362,    3653 },
    { 53392,    3130 },
    { 53393,    3386 },
    { 53394,    3642 },
    { 53424,    3134 },
    { 53425,    3390 },
    { 53426,    3646 },
    { 53456,    3138 },
    { 53457,    3394 },
    { 53458,    3421 },
    { 53488,    3142 },
    { 53489,    3398 },
    { 53490,    3654 },
    { 53520,    3131 },
    { 53521,    3385 },
    { 53522,    3643 },
    { 53552,    3135 },
    { 53553,    3389 },
    { 53554,    3647 },
    { 53584,    3139 },
    { 53585,    3393 },
    { 53586,    3651 },
    { 53616,    3143 },
    { 53617,    3397 },
    { 53618,    3655 },
    { 53648,    3132 },
    { 53649,    3229 },
    { 53650,    3644 },
    { 53680,    3136 },
    { 53681,    3392 },
    { 53682,    3648 },
    { 53712,    3140 },
    { 53713,    3396 },
    { 53714,    3652 },
    { 53744,    3144 },
    { 53745,    3400 },
    { 53746,    3656 },
    { 53776,    3145 },
    { 53777,    3401 },
    { 53778,    3066 },
    { 53808,    3149 },
    { 53809,    3405 },
    { 53810,    3661 },
    { 53840,    3153 },
    { 53841,    3409 },
    { 53842,    3665 },
    { 53872,    3157 },
    { 53873,    3413 },
    { 53874,    3669 },
    { 53904,    3146 },
    { 53905,    3402 },
    { 53906,    3658 },
    { 53936,    3150 },
    { 53937,    3406 },
    { 53938,    3662 },
    { 53968,    3154 },
    { 53969,    3410 },
    { 53970,    3666 },
    { 54000,    3158 },
    { 54001,    3414 },
    { 54002,    3670 },
    { 54032,    3147 },
    { 54033,    3401 },
    { 54034,    3659 },
    { 54064,    3151 },
    { 54065,    3405 },
    { 54066,    3030 },
    { 54096,    3155 },
    { 54097,    3409 },
    { 54098,    3473 },
    { 54128,    3159 },
    { 54129,    3413 },
    { 54130,    3671 },
    { 54160,    3148 },
    { 54161,    3404 },
    { 54162,    3660 },
    { 54192,    3152 },
    { 54193,    3408 },
    { 54194,    3664 },
    { 54224,    3156 },
    { 54225,    3412 },
    { 54226,    3668 },
    { 54256,    3160 },
    { 54257,    3416 },
    { 54258,    3672 },
    { 54288,    3161 },
    { 54289,    3417 },
    { 54290,    3673 },
    { 54320,    3165 },
    { 54321,    3421 },
    { 54322,    3371 },
    { 54352,    3169 },
    { 54353,    3425 },
    { 54354,    3681 },
    { 54384,    3173 },
    { 54385,    3429 },
    { 54386,    3685 },
    { 54416,    3162 },
    { 54417,    3418 },
    { 54418,    3674 },
    { 54448,    3166 },
    { 54449,    3422 },
    { 54480,    3170 },
    { 54481,    3426 },
    { 54482,    3682 },
    { 54512,    3174 },
    { 54513,    3430 },
    { 54514,    3686 },
    { 54544,    3163 },
    { 54545,    3417 },
    { 54546,    3675 },
    { 54576,    3167 },
    { 54577,    3421 },
    { 54578,    3679 },
    { 54608,    3171 },
    { 54609,    3425 },
    { 54610,    3683 },
    { 54640,    3175 },
    { 54641,    3429 },
    { 54642,    3687 },
    { 54672,    3164 },
    { 54673,    3420 },
    { 54674,    3676 },
    { 54704,    3168 },
    { 54705,    3424 },
    { 54706,    3680 },
    { 54736,    3172 },
    { 54737,    3428 },
    { 54738,    3260 },
    { 54768,    3176 },
    { 54769,    3432 },
    { 54770,    3688 },
    { 54800,    3153 },
    { 54801,    3433 },
    { 54802,    3138 },
    { 54832,    3181 },
    { 54833,    3161 },
    { 54834,    3693 },
    { 54864,    3185 },
    { 54865,    3441 },
    { 54866,    3697 },
    { 54896,    3189 },
    { 54897,    3445 },
    { 54898,    3226 },
    { 54928,    3154 },
    { 54929,    3434 },
    { 54930,    3690 },
    { 54960,    3182 },
    { 54961,    3438 },
    { 54962,    3694 },
    { 54992,    3186 },
    { 54993,    3442 },
    { 54994,    3698 },
    { 55024,    3190 },
    { 55025,    3446 },
    { 55026,    3082 },
    { 55056,    3155 },
    { 55057,    3433 },
    { 55058,    3691 },
    { 55088,    3183 },
    { 55089,    3161 },
    { 55090,    3695 },
    { 55120,    3187 },
    { 55121,    3441 },
    { 55122,    3699 },
    { 55152,    3191 },
    { 55153,    3445 },
    { 55154,    3703 },
    { 55184,    3156 },
    { 55186,    3692 },
    { 55216,    3184 },
    { 55217,    3440 },
    { 55218,    3696 },
    { 55248,    3188 },
    { 55249,    3444 },
    { 55250,    3700 },
    { 55280,    3192 },
    { 55281,    3448 },
    { 55312,    3193 },
    { 55313,    3449 },
    { 55314,    3147 },
    { 55344,    3197 },
    { 55345,    3453 },
    { 55346,    3709 },
    { 55376,    3201 },
    { 55377,    3457 },
    { 55378,    3713 },
    { 55408,    3205 },
    { 55409,    3461 },
    { 55410,    3717 },
    { 55440,    3194 },
    { 55441,    3450 },
    { 55442,    3706 },
    { 55472,    3198 },
    { 55473,    3454 },
    { 55474,    3041 },
    { 55504,    3202 },
    { 55505,    3458 },
    { 55506,    3714 },
    { 55536,    3206 },
    { 55537,    3462 },
    { 55538,    3718 },
    { 55568,    3195 },
    { 55569,    3451 },
    { 55570,    3707 },
    { 55600,    3199 },
    { 55601,    3455 },
    { 55602,    3711 },
    { 55632,    3203 },
    { 55633,    3459 },
    { 55634,    3715 },
    { 55664,    3207 },
    { 55665,    3463 },
    { 55666,    3719 },
    { 55696,    3196 },
    { 55697,    3005 },
    { 55698,    3708 },
    { 55728,    3200 },
    { 55729,    3456 },
    { 55730,    3712 },
    { 55760,    3204 },
    { 55761,    3460 },
    { 55762,    3222 },
    { 55792,    3208 },
    { 55793,    3292 },
    { 55794,    3239 },
    { 55824,    3021 },
    { 55825,    3465 },
    { 55826,    3721 },
    { 55856,    3017 },
    { 55857,    3469 },
    { 55858,    3725 },
    { 55888,    3217 },
    { 55889,    3473 },
    { 55890,    3386 },
    { 55920,    3221 },
    { 55921,    3477 },
    { 55922,    3733 },
    { 55952,    3022 },
    { 55953,    3048 },
    { 55954,    3722 },
    { 55984,    3018 },
    { 55985,    3470 },
    { 55986,    3726 },
    { 56016,    3218 },
    { 56017,    3474 },
    { 56018,    3730 },
    { 56048,    3222 },
    { 56049,    3478 },
    { 56050,    3183 },
    { 56080,    3211 },
    { 56081,    3467 },
    { 56082,    3723 },
    { 56112,    3215 },
    { 56113,    3471 },
    { 56114,    3727 },
    { 56144,    3219 },
    { 56145,    3475 },
    { 56146,    3731 },
    { 56176,    3223 },
    { 56177,    3479 },
    { 56178,    3735 },
    { 56208,    3024 },
    { 56209,    3468 },
    { 56210,    3724 },
    { 56240,    3020 },
    { 56241,    3472 },
    { 56242,    3728 },
    { 56272,    3220 },
    { 56273,    3476 },
    { 56274,    3732 },
    { 56304,    3224 },
    { 56305,    3480 },
    { 56306,    3736 },
    { 56336,    3225 },
    { 56337,    3481 },
    { 56338,    3737 },
    { 56368,    3229 },
    { 56369,    3485 },
    { 56370,    3741 },
    { 56400,    3233 },
    { 56401,    3489 },
    { 56402,    3745 },
    { 56432,    3237 },
    { 56433,    3493 },
    { 56434,    3749 },
    { 56464,    3226 },
    { 56465,    3482 },
    { 56466,    3738 },
    { 56496,    3230 },
    { 56497,    3486 },
    { 56498,    3742 },
    { 56528,    3234 },
    { 56529,    3490 },
    { 56530,    3160 },
    { 56560,    3238 },
    { 56561,    3494 },
    { 56562,    3424 },
    { 56592,    3227 },
    { 56593,    3483 },
    { 56594,    3420 },
    { 56624,    3231 },
    { 56625,    3487 },
    { 56626,    3743 },
    { 56656,    3235 },
    { 56657,    3120 },
    { 56658,    3747 },
    { 56688,    3239 },
    { 56689,    3495 },
    { 56690,    3751 },
    { 56720,    3228 },
    { 56721,    3272 },
    { 56722,    3740 },
    { 56752,    3232 },
    { 56753,    3488 },
    { 56754,    3744 },
    { 56784,    3236 },
    { 56785,    3492 },
    { 56786,    3748 },
    { 56816,    3240 },
    { 56817,    3260 },
    { 56818,    3752 },
    { 56848,    3217 },
    { 56849,    3013 },
    { 56850,    3753 },
    { 56880,    3245 },
    { 56881,    3501 },
    { 56882,    3162 },
    { 56912,    3249 },
    { 56913,    3505 },
    { 56914,    3761 },
    { 56944,    3253 },
    { 56945,    3509 },
    { 56946,    3765 },
    { 56976,    3218 },
    { 56977,    3498 },
    { 56978,    3754 },
    { 57008,    3246 },
    { 57009,    3502 },
    { 57010,    3758 },
    { 57040,    3250 },
    { 57041,    3506 },
    { 57042,    3762 },
    { 57072,    3254 },
    { 57073,    3510 },
    { 57074,    3766 },
    { 57104,    3219 },
    { 57105,    3499 },
    { 57106,    3755 },
    { 57136,    3247 },
    { 57137,    3133 },
    { 57138,    3759 },
    { 57168,    3251 },
    { 57169,    3507 },
    { 57200,    3255 },
    { 57201,    3511 },
    { 57202,    3269 },
    { 57232,    3220 },
    { 57233,    3172 },
    { 57234,    3756 },
    { 57264,    3248 },
    { 57265,    3504 },
    { 57266,    3760 },
    { 57296,    3252 },
    { 57297,    3508 },
    { 57298,    3764 },
    { 57328,    3256 },
    { 57329,    3512 },
    { 57330,    3768 },

	{ 49648,    3016 },
	{ 51696,    3080 },
	{ 53744,    3144 },
	{ 55792,    3208 },
	{ 50160,    3032 },
	{ 52208,    3096 },
	{ 54256,    3160 },
	{ 56304,    3224 },
	{ 50672,    3048 },
	{ 52720,    3112 },
	{ 54768,    3176 },
	{ 56816,    3240 },
	{ 51184,    3064 },
	{ 53232,    3128 },
	{ 55280,    3192 },
	{ 57328,    3256 },
	{ 49649,    3272 },
	{ 51697,    3336 },
	{ 53745,    3400 },
	{ 55793,    3464 },
	{ 50161,    3288 },
	{ 52209,    3352 },
	{ 54257,    3416 },
	{ 56305,    3480 },
	{ 50673,    3304 },
	{ 52721,    3368 },
	{ 54769,    3432 },
	{ 56817,    3496 },
	{ 51185,    3320 },
	{ 53233,    3384 },
	{ 55281,    3448 },
	{ 57329,    3512 },
	{ 49650,    3528 },
	{ 51698,    3592 },
	{ 53746,    3656 },
	{ 55794,    3720 },
	{ 50162,    3544 },
	{ 52210,    3608 },
	{ 54258,    3672 },
	{ 56306,    3736 },
	{ 50674,    3560 },
	{ 52722,    3624 },
	{ 54770,    3688 },
	{ 56818,    3752 },
	{ 51186,    3576 },
	{ 53234,    3640 },
	// { 55282,    3704 }, ダメ顔
	{ 57330,    3768 }
};

/*
 DDraw.dllの中で、男性の合成顔時に姫の顔が出現するバグを解消する。

5BE724B0   8B4424 04        MOV     EAX, DWORD PTR SS:[ESP+4]
5BE724B4   83EC 08          SUB     ESP, 8
5BE724B7   53               PUSH    EBX
5BE724B8   C70485 083B475C >MOV     DWORD PTR DS:[EAX*4+5C473B08], 0
5BE724C3   C1E0 06          SHL     EAX, 6
5BE724C6   8BD8             MOV     EBX, EAX
5BE724C8   69DB 003C0000    IMUL    EBX, EBX, 3C00
5BE724CE   56               PUSH    ESI
5BE724CF   68 1CF3E75B      PUSH    DDraw.5BE7F31C                   ; ASCII "kao_24bit\000"
5BE724D4   68 00100000      PUSH    1000
5BE724D9   68 6042475C      PUSH    DDraw.5C474260                   ; ASCII "C:\WINDOWS\system32\ddraw.dll"
5BE724DE   8BF1             MOV     ESI, ECX						← ECXが顔番号+1
5BE724E0   894424 20        MOV     DWORD PTR SS:[ESP+20], EAX
5BE724E4   C70485 402DED5B >MOV     DWORD PTR DS:[EAX*4+5BED2D40], ->
5BE724EF   81C3 4035ED5B    ADD     EBX, DDraw.5BED3540
*/

#define DDRAW_DLL_ENTRYPOINT_TO_PUSH_1000						 0x24d4					// DDrawの先頭から、上記の「5BE724D4   68 00100000      PUSH    1000」の先頭相当するアドレス


/*

┌顔。左上が0x0　右下が0xF 1は左上の１つ下。
　　　　　│
	 0b1100000[1111][000][00]
	 　　　　　│　　 │ └ 行の中で４つのどれなのか
		  　　　　　└16顔 └猛将・知将などの行

			   0b1100000[1111][001]00 知将

			   0b1100000[1111][010]00 謀将

			   0b1100000[1111][011]00 凡将

			   0b1100000[1111][100]00 姫

			   0b110[01][00][0111][101]00


			   0b1100000[1111][101]00 鎧兜若者〜平服若者

			   0b1100000[1111][110]00 平服壮年〜剣豪

			   0b1100000[1111][111]00 茶人僧侶

			   ┌目。０〜３
			   0b11000[00][0000][000][00]

			   ┌口。０〜３
			   0b110[00]00[0000][000][00]
			   */


// iGeneralFaceBaseType=上のどれか
// iMouse 0〜3
// iEye 0〜3
// iFaceID 0〜15。顔が１６個あるのは、

//   00 04 08 12
//   01 05 09 13
//   02 06 10 14
//   03 07 11 15


// 0b1100000[1111][100]00 姫
struct HIME_FACE_STRUCT {
	byte kasou2 : 3; // 0b110
	byte mouse : 2; // 0〜3
	byte eye : 2; // 0〜3
	byte kasou : 4; // 0b1111
	byte hime : 3; // 0b100
	byte face_type : 2;
};

// 今から描画しようとする顔ID
int iFaceIDOfOnDrawKaoFullColorFace = -1; // (顔番号より１つ大きな値が返ってくる。ちょうどDDraw.dllが取り扱っている.bmpの番号)
void OnDrawKaoFullColorFaceExecute() {

	// 姫系の顔だ
	if ( 3001 <= iFaceIDOfOnDrawKaoFullColorFace && iFaceIDOfOnDrawKaoFullColorFace <= 3768 ) {

		// OutputDebugStream("DDrawから渡ってきた番号 %d", iFaceIDOfOnDrawKaoFullColorFace);

		// DDraw.dll の.bmpタイプから、通常の顔番号への変換
		vector<int> iBindedFaceList; // 本来ならfaceID⇔bmpで１対１なのだが、ddraw.dllがバグっていて、複数の天翔記合成顔に対して、同じbmpを出してしまっている。
									 // よってもとに戻すには、bmp１つに対して、複数の天翔記合成顔が対応し得る。

		if (iBudhouNodOfPathDDrawHimekaoApeearBug > 0) {
			int iBushouID = iBudhouNodOfPathDDrawHimekaoApeearBug - 1;
			bool isFaceBushouExist = false;
			if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

				/*
					0b1100000[1111][001]00 知将
					0b1100000[1111][010]00 謀将
		            0b1100000[1111][011]00 凡将
					0b1100000[1111][100]00 姫
				*/
				int isHime = iExterniFaceIdOffGeneralKaoFaceExecuteNormalizedFaceID & 0x1c; // 0x1c == 0b11100
				int isKasou = iExterniFaceIdOffGeneralKaoFaceExecuteNormalizedFaceID & 0xc000; // 0x1c == 0b1100000[1111][100]00 仮想
				// OutputDebugStream("★ %d", iExterniFaceIdOffGeneralKaoFaceExecuteNormalizedFaceID);
				// OutputDebugStream("★ %d", isHime);
				// OutputDebugStream("★ %d", isKasou);
				if (isHime == 16 && isKasou == 0xc000) { // 0b1100000[1111][100]00 姫
					// OutputDebugStream("仮想姫だよ");
					isFaceBushouExist = true;
				}
				else {
					// OutputDebugStream("違うよ");
				}

			}
			// そんな奴はおらんｗ
			if (!isFaceBushouExist) {
				// OutputDebugStream("居ない");
				iFaceIDOfOnDrawKaoFullColorFace = 9999; // DDrawへの描画は存在しない番号に変更する。
			}
			else {
				// OutputDebugStream("居る");
			}
		}

		else {
		}

		/*
		for (int i = 365; i < 368; i++) {
			OutputDebugStream("スロット%dの顔番号は %d", i, nb6bushouname[i].number);
		}
		bool isFaceBushouExist = true;
		for (int h = 0; h <= 2; h++) {
			for (int x = 0; x <= 3; x++) {
				for (int y = 0; y <= 3; y++) {
					int id = getFactoryGeneralKaoFace(GeneralFaceBaseType::姫一 + h, 15, x, y);
					int faceid = 3000 + y * 64 + x * 16 + h * 256 + 16;
					OutputDebugStream("顔タイプ:%d 目:%d 口:%d 顔番号%d グラ%d", h, x, y, id, faceid);
				}
			}
		}
		*/

	}
}

// DDrawオリジナルハンドル
extern HINSTANCE hDDrawMod;

int pDDrawJumpFromToOnDrawKaoFullColorFace    = NULL; 
int pDDrawReturnLblFromOnDrawKaoFullColorFace = NULL; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

__declspec( naked ) void WINAPI OnDrawKaoFullColorFace() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		push 0x1000

		// ECXを顔番号として保存しておく
		mov iFaceIDOfOnDrawKaoFullColorFace, ecx

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnDrawKaoFullColorFaceExecute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// ecxを上書きする。
		mov ecx, iFaceIDOfOnDrawKaoFullColorFace

		jmp pDDrawReturnLblFromOnDrawKaoFullColorFace
	}
}


char cmdOnDDrawJumpFromwKaoFullColorFace[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void patchDisableDDrawHimeWrongAppearBug() {
	pDDrawJumpFromToOnDrawKaoFullColorFace = (int)hDDrawMod + DDRAW_DLL_ENTRYPOINT_TO_PUSH_1000; // DDrawの先頭から、// DDrawの先頭から、上記の「5BE724D4   68 00100000      PUSH    1000」の先頭相当するアドレス
	pDDrawReturnLblFromOnDrawKaoFullColorFace = pDDrawJumpFromToOnDrawKaoFullColorFace+5; // １行分の命令の次の場所が返る場所

	// まずアドレスを数字として扱う
	int iAddress = (int)OnDrawKaoFullColorFace;
	int SubAddress = iAddress - ( pDDrawJumpFromToOnDrawKaoFullColorFace + 5 );
		// ５というのは、
	memcpy(cmdOnDDrawJumpFromwKaoFullColorFace+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pDDrawJumpFromToOnDrawKaoFullColorFace), cmdOnDDrawJumpFromwKaoFullColorFace, 5, NULL); //5バイトのみ書き込む
}
