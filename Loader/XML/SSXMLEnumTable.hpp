#pragma once
#include <Siv3D.hpp>
#include "../../Common/SSTypeDefine.hpp"

namespace s3d::SpriteStudio
{
	constexpr std::array<std::pair<const char32_t*, TextureAddressMode>, 3> TextureAddressModeTable =
	{
		std::pair{ U"clamp" , TextureAddressMode::Clamp  },
		std::pair{ U"repeat", TextureAddressMode::Repeat },
		std::pair{ U"Mirror", TextureAddressMode::Mirror },
	};

	constexpr std::array<std::pair<const char32_t*, TextureFilter>, 2> TextureFilterTable =
	{
		std::pair{ U"linear" , TextureFilter::Linear  },
		std::pair{ U"nearest", TextureFilter::Nearest },
	};

	constexpr std::array<std::pair<const char32_t*, PartsSortMode>, 2> PartsSortModeTable =
	{
		std::pair{ U"prio", PartsSortMode::Prio },
		std::pair{ U"z"   , PartsSortMode::Z    },
	};

	constexpr std::array<std::pair<const char32_t*, PartType>, 14> PartTypeTable =
	{
		std::pair{ U"null"      , PartType::Null       },
		std::pair{ U"normal"    , PartType::Normal     },
		std::pair{ U"text"      , PartType::Text       },
		std::pair{ U"instance"  , PartType::Instance   },
		std::pair{ U"armature"  , PartType::Armature   },
		std::pair{ U"effect"    , PartType::Effect     },
		std::pair{ U"mesh"      , PartType::Mesh       },
		std::pair{ U"movenode"  , PartType::Movenode   },
		std::pair{ U"constraint", PartType::Constraint },
		std::pair{ U"mask"      , PartType::Mask       },
		std::pair{ U"joint"     , PartType::Joint      },
		std::pair{ U"bonepoint" , PartType::Bonepoint  },
		std::pair{ U"shape"     , PartType::Shape      },
		std::pair{ U"nines"     , PartType::Nineslice  },
	};

	constexpr std::array<std::pair<const char32_t*, BoundsType>, 6> BoundsTypeTable =
	{
		std::pair{ U"none"       , BoundsType::None           },
		std::pair{ U"quad"       , BoundsType::Quad           },
		std::pair{ U"aabb"       , BoundsType::AABB           },
		std::pair{ U"circle"     , BoundsType::Circle         },
		std::pair{ U"circle_smin", BoundsType::CircleScaleMin },
		std::pair{ U"circle_smax", BoundsType::CircleScaleMax },
	};

	constexpr std::array<std::pair<const char32_t*, InheritType>, 2> InheritTypeTable =
	{
		std::pair{ U"parent", InheritType::Parent },
		std::pair{ U"self"  , InheritType::Self   },
	};

	constexpr std::array<std::pair<const char32_t*, BlendType>, 10> BlendTypeTable =
	{
		std::pair{ U"mix"      , BlendType::Mix       },
		std::pair{ U"Mix"      , BlendType::Mix       }, // sseeのBehaviorがMix・・・
		std::pair{ U"mul"      , BlendType::Mul       },
		std::pair{ U"add"      , BlendType::Add       },
		std::pair{ U"Add"      , BlendType::Add       }, // sseeのBehaviorがAdd・・・
		std::pair{ U"sub"      , BlendType::Sub       },
		std::pair{ U"mulalpha" , BlendType::MulAlpha  },
		std::pair{ U"screen"   , BlendType::Screen    },
		std::pair{ U"exclusion", BlendType::Exclusion },
		std::pair{ U"invert"   , BlendType::Invert    },
	};

	constexpr std::array<std::pair<const char32_t*, ColorBlendTarget>, 2> ColorBlendTargetTable =
	{
		std::pair{ U"whole" , ColorBlendTarget::Whole  },
		std::pair{ U"vertex", ColorBlendTarget::Vertex },
	};

	constexpr std::array<std::pair<const char32_t*, InterpolationType>, 24> InterpolationTypeTable =
	{
		std::pair{ U"none"                , InterpolationType::None                 },
		std::pair{ U"linear"              , InterpolationType::Linear               },
		std::pair{ U"hermite"             , InterpolationType::Hermite              },
		std::pair{ U"bezier"              , InterpolationType::Bezier               },
		std::pair{ U"acceleration"        , InterpolationType::Acceleration         },
		std::pair{ U"deceleration"        , InterpolationType::Deceleration         },
		std::pair{ U"easeIn"              , InterpolationType::EaseIn               },
		std::pair{ U"easeOut"             , InterpolationType::EaseOut              },
		std::pair{ U"easeInOut"           , InterpolationType::EaseInOut            },
		std::pair{ U"easeExponentialIn"   , InterpolationType::EaseExponentialIn    },
		std::pair{ U"easeExponentialOut"  , InterpolationType::EaseExponentialOut   },
		std::pair{ U"easeExponentialInOut", InterpolationType::EaseExponentialInOut },
		std::pair{ U"easeSineIn"          , InterpolationType::EaseSineIn           },
		std::pair{ U"easeSineOut"         , InterpolationType::EaseSineOut          },
		std::pair{ U"easeSineInOut"       , InterpolationType::EaseSineInOut        },
		std::pair{ U"easeElasticIn"       , InterpolationType::EaseElasticIn        },
		std::pair{ U"easeElasticOut"      , InterpolationType::EaseElasticOut       },
		std::pair{ U"easeElasticInOut"    , InterpolationType::EaseElasticInOut     },
		std::pair{ U"easeBounceIn"        , InterpolationType::EaseBounceIn         },
		std::pair{ U"easeBounceOut"       , InterpolationType::EaseBounceOut        },
		std::pair{ U"easeBounceInOut"     , InterpolationType::EaseBounceInOut      },
		std::pair{ U"easeBackIn"          , InterpolationType::EaseBackIn           },
		std::pair{ U"easeBackOut"         , InterpolationType::EaseBackOut          },
		std::pair{ U"easeBackInOut"       , InterpolationType::EaseBackInOut        },
	};

	constexpr std::array<std::pair<const char32_t*, AttributeKind>, AttributeKindNum> AttributeKindTable =
	{
		std::pair{ U"CELL", AttributeKind::Cell          },
		std::pair{ U"POSX", AttributeKind::PosX          },
		std::pair{ U"POSY", AttributeKind::PosY          },
		std::pair{ U"POSZ", AttributeKind::PosZ          },
		std::pair{ U"ROTX", AttributeKind::RotateX       },
		std::pair{ U"ROTY", AttributeKind::RotateY       },
		std::pair{ U"ROTZ", AttributeKind::RotateZ       },
		std::pair{ U"SCLX", AttributeKind::ScaleX        },
		std::pair{ U"SCLY", AttributeKind::ScaleY        },
		std::pair{ U"LSCX", AttributeKind::LocalScaleX   },
		std::pair{ U"LSCY", AttributeKind::LocalScaleY   },
		std::pair{ U"ALPH", AttributeKind::Alpha         },
		std::pair{ U"LALP", AttributeKind::LocalAlpha    },
		std::pair{ U"PRIO", AttributeKind::Priority      },
		std::pair{ U"FLPH", AttributeKind::FlipH         },
		std::pair{ U"FLPV", AttributeKind::FlipV         },
		std::pair{ U"HIDE", AttributeKind::Hide          },
		std::pair{ U"PCOL", AttributeKind::PartsColor    },
		std::pair{ U"VCOL", AttributeKind::VertexColor   },
		std::pair{ U"SHDR", AttributeKind::Shader        },
		std::pair{ U"VERT", AttributeKind::Vertex        },
		std::pair{ U"PVTX", AttributeKind::PivotX        },
		std::pair{ U"PVTY", AttributeKind::PivotY        },
		std::pair{ U"ANCX", AttributeKind::AnchorX       },
		std::pair{ U"ANCY", AttributeKind::AnchorY       },
		std::pair{ U"SIZX", AttributeKind::SizeX         },
		std::pair{ U"SIZY", AttributeKind::SizeY         },
		std::pair{ U"IFLH", AttributeKind::ImageFlipH    },
		std::pair{ U"IFLV", AttributeKind::ImageFlipV    },
		std::pair{ U"UVTX", AttributeKind::UVTranslateX  },
		std::pair{ U"UVTY", AttributeKind::UVTranslateY  },
		std::pair{ U"UVRZ", AttributeKind::UVRotateZ     },
		std::pair{ U"UVSX", AttributeKind::UVScaleX      },
		std::pair{ U"UVSY", AttributeKind::UVScaleY      },
		std::pair{ U"BNDR", AttributeKind::BoundRadius   },
		std::pair{ U"MASK", AttributeKind::Mask          },
		std::pair{ U"USER", AttributeKind::User          },
		std::pair{ U"SIGN", AttributeKind::Signal        },
		std::pair{ U"IPRM", AttributeKind::InstanceParam },
		std::pair{ U"EFCT", AttributeKind::Effect        },
		std::pair{ U"DEFM", AttributeKind::Deform        },
		std::pair{ U"ADIO", AttributeKind::Audio         },
		std::pair{ U"TCHG", AttributeKind::TextureChange },
	};

	constexpr std::array<std::pair<const char32_t*, IKRotationArrow>, 3> IKRotationArrowTable =
	{
		std::pair{ U"arrowfree"    , IKRotationArrow::ArrowFree     },
		std::pair{ U"crockwise"    , IKRotationArrow::ClockWise     },
		std::pair{ U"anticlockwise", IKRotationArrow::AntiClockWise },
	};

	constexpr std::array<std::pair<const char32_t*, EffectNodeType>, 3> EffectNodeTable =
	{
		std::pair{ U"Root"    , EffectNodeType::Root     },
		std::pair{ U"Emmiter" , EffectNodeType::Emitter  },
		std::pair{ U"Particle", EffectNodeType::Particle },
	};

	constexpr std::array<std::pair<const char32_t*, SignalType>, 4> SignalTypeTable =
	{
		std::pair{ U"none"    , SignalType::None    },
		std::pair{ U"index"   , SignalType::Index   },
		std::pair{ U"integer" , SignalType::Integer },
		std::pair{ U"floating", SignalType::Float   },
	};
}
