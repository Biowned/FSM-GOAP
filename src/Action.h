#pragma once

#include <string>
#include <unordered_map>

namespace goap {
	struct WorldState;

	class Action {
	private:
		std::string name_; 
		int cost_;        					   
		std::unordered_map<int, bool> preconditions_;
		std::unordered_map<int, bool> effects_;

	public:
		Action();
		Action(std::string name, int cost);

		/**
		Is this action eligible to operate on the given worldstate?
		@param ws the worldstate in question
		@return true if this worldstate meets the preconditions
		*/
		bool operableOn(const goap::WorldState& ws) const;

		/**
		Act on the given worldstate. Will not check for "eligiblity" and will happily
		act on whatever worldstate you provide it.
		@param the worldstate to act on
		@return a copy worldstate, with effects applied
		*/
		WorldState actOn(const WorldState& ws) const;

		/**
		Set the given precondition variable and value.
		@param key the name of the precondition
		@param value the value the precondition must hold
		*/
		void setPrecondition(const int key, const bool value) {
			preconditions_[key] = value;
		}

		/**
		Set the given effect of this action, in terms of variable and new value.
		@param key the name of the effect
		@param value the value that will result
		*/
		void setEffect(const int key, const bool value) {
			effects_[key] = value;
		}

		int cost() const { return cost_; }

		std::string name() const { return name_; }
	};

}
